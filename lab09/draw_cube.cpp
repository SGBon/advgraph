/*
 * Vulkan Samples
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
VULKAN_SAMPLE_SHORT_DESCRIPTION
Draw Cube
*/

/* This is part of the draw cube progression */

#include <util_init.hpp>
#include <assert.h>
#include <string.h>
#include <cstdlib>
#include "cube_data.h"

float *vBuffer;
VkBuffer iBuffer;

void mesh_init(struct sample_info &info){
  int i;
  int k;
  VkResult U_ASSERT_ONLY res;
  bool U_ASSERT_ONLY pass;

  float vertices[8][4] = {
    {-1.0f,-1.0f,-1.0f,1.0f},  //0
    {-1.0f,-1.0f, 1.0f,1.0f},  //1
    {-1.0f, 1.0f,-1.0f,1.0f},  //2
    {-1.0f, 1.0f, 1.0f,1.0f},  //3
    { 1.0f,-1.0f,-1.0f,1.0f},  //4
    { 1.0f,-1.0f, 1.0f,1.0f},  //5
    { 1.0f, 1.0f,-1.0f,1.0f},  //6
    { 1.0f, 1.0f, 1.0f,1.0f}  //7
  };

  float normals[8][3] = {
    {-1.0f,-1.0f,-1.0f},  //0
    {-1.0f,-1.0f, 1.0f},  //1
    {-1.0f, 1.0f,-1.0f},  //2
    {-1.0f, 1.0f, 1.0f},  //3
    { 1.0f,-1.0f,-1.0f},  //4
    { 1.0f,-1.0f, 1.0f},  //5
    { 1.0f, 1.0f,-1.0f},  //6
    { 1.0f, 1.0f, 1.0f}  //7
  };

  uint16_t indices[36] = {3,1,0,0,2,3,
    0,4,5,5,1,0,
    2,6,7,7,3,2,
    0,4,6,0,2,6,
    1,5,7,7,3,1,
    7,5,4,4,6,7};

  vBuffer = new float[2*8*4];
  k = 0;
  for(i = 0;i<8;i++){
    vBuffer[k++] = vertices[i][0];
    vBuffer[k++] = vertices[i][1];
    vBuffer[k++] = vertices[i][2];
    vBuffer[k++] = vertices[i][3];
    vBuffer[k++] = normals[i][0];
    vBuffer[k++] = normals[i][1];
    vBuffer[k++] = normals[i][2];
    vBuffer[k++] = 1.0;
  }

  VkBufferCreateInfo buf_info = {};
  buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buf_info.pNext = NULL;
  buf_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  buf_info.size = sizeof(indices);
  buf_info.queueFamilyIndexCount = 0;
  buf_info.pQueueFamilyIndices = NULL;
  buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  buf_info.flags = 0;
  res = vkCreateBuffer(info.device, &buf_info, NULL, &iBuffer);
  assert(res == VK_SUCCESS);

  VkMemoryRequirements mem_reqs;
  vkGetBufferMemoryRequirements(info.device,iBuffer,&mem_reqs);

  VkMemoryAllocateInfo alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.pNext = NULL;
  alloc_info.memoryTypeIndex = 0;
  alloc_info.allocationSize = mem_reqs.size;
  pass = memory_type_from_properties(info, mem_reqs.memoryTypeBits,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |  VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    &alloc_info.memoryTypeIndex);
  assert(pass && "No mappable coherent memory");

  VkDeviceMemory mem;
  res = vkAllocateMemory(info.device,&alloc_info,NULL,&mem);
  assert(res == VK_SUCCESS);

  uint8_t *pData;
  res = vkMapMemory(info.device,mem,0,mem_reqs.size,0,(void**) &pData);
  assert(res == VK_SUCCESS);

  memcpy(pData,indices,sizeof(indices));

  vkUnmapMemory(info.device,mem);

  res = vkBindBufferMemory(info.device, iBuffer, mem, 0);
  assert(res == VK_SUCCESS);
}

/* For this sample, we'll start with GLSL so the shader function is plain */
/* and then use the glslang GLSLtoSPV utility to convert it to SPIR-V for */
/* the driver.  We do this for clarity rather than using pre-compiled     */
/* SPIR-V                                                                 */

static const char *vertShaderText =
    "#version 400\n"
    "#extension GL_ARB_separate_shader_objects : enable\n"
    "#extension GL_ARB_shading_language_420pack : enable\n"
    "layout (std140, binding = 0) uniform bufferVals {\n"
    "    mat4 mvp;\n"
    "} myBufferVals;\n"
    "layout (location = 0) in vec4 pos;\n"
    "layout (location = 1) in vec4 vNormal;\n"
    "layout (location = 0) out vec4 fragNormal;\n"
    "out gl_PerVertex { \n"
    "    vec4 gl_Position;\n"
    "};\n"
    "void main() {\n"
    "   fragNormal = vNormal;\n"
    "   gl_Position = myBufferVals.mvp * pos;\n"
    "}\n";

static const char *fragShaderText =
    "#version 400\n"
    "#extension GL_ARB_separate_shader_objects : enable\n"
    "#extension GL_ARB_shading_language_420pack : enable\n"
    "layout (location = 0) in vec4 fragNormal;\n"
    "layout (location = 0) out vec4 outColor;\n"
    "void main() {\n"
    "   vec4 base = vec4(1.0,0.0,0.0,1.0);\n"
    "   vec4 white = vec4(1.0,1.0,1.0,1.0);\n"
    "   float diffuse;\n"
    "   vec3 L = vec3(1.0,-1.0,0.0);\n"
    "   vec3 N;\n"
    "   vec3 R = normalize(L+vec3(0.0,1.0,1.0));\n"
    "   float specular;\n"
    "   N = normalize(fragNormal.xyz);\n"
    "   diffuse = dot(N,L);\n"
    "   if(diffuse < 0.0){\n"
    "     diffuse = 0.0;\n"
    "     specular = 0.0;\n"
    "   } else {\n"
    "     specular = pow(max(0.0,dot(N,R)),100.0);\n"
    "   }\n"
    "   outColor = min(base*0.3 + 0.7*base*diffuse + 0.7*white*specular,vec4(1.0));\n"
    "   outColor.a = base.a;\n"
    "}\n";

int sample_main(int argc, char *argv[]) {
    VkResult U_ASSERT_ONLY res;
    struct sample_info info = {};
    char sample_title[] = "Draw Cube";
    const bool depthPresent = true;

    process_command_line_args(info, argc, argv);
    init_global_layer_properties(info);
    init_instance_extension_names(info);
    init_device_extension_names(info);
    init_instance(info, sample_title);
    init_enumerate_device(info);
    init_window_size(info, 500, 500);
    init_connection(info);
    init_window(info);
    init_swapchain_extension(info);
    init_device(info);

    init_command_pool(info);
    init_command_buffer(info);
    execute_begin_command_buffer(info);
    init_device_queue(info);
    init_swap_chain(info);
    init_depth_buffer(info);
    init_uniform_buffer(info);
    init_descriptor_and_pipeline_layouts(info, false);
    init_renderpass(info, depthPresent);
    init_shaders(info, vertShaderText, fragShaderText);
    init_framebuffers(info, depthPresent);
    /*
    init_vertex_buffer(info, g_vb_solid_face_colors_Data,
                       sizeof(g_vb_solid_face_colors_Data),
                       sizeof(g_vb_solid_face_colors_Data[0]), false);
    */
    mesh_init(info);
    init_vertex_buffer(info,vBuffer,2*8*4*sizeof(float),
  2*4*sizeof(float),false);
    init_descriptor_pool(info, false);
    init_descriptor_set(info, false);
    init_pipeline_cache(info);
    init_pipeline(info, depthPresent);

    /* VULKAN_KEY_START */

    VkClearValue clear_values[2];
    clear_values[0].color.float32[0] = 0.2f;
    clear_values[0].color.float32[1] = 0.2f;
    clear_values[0].color.float32[2] = 0.2f;
    clear_values[0].color.float32[3] = 0.2f;
    clear_values[1].depthStencil.depth = 1.0f;
    clear_values[1].depthStencil.stencil = 0;

    VkSemaphore imageAcquiredSemaphore;
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
    imageAcquiredSemaphoreCreateInfo.sType =
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;
    imageAcquiredSemaphoreCreateInfo.flags = 0;

    res = vkCreateSemaphore(info.device, &imageAcquiredSemaphoreCreateInfo,
                            NULL, &imageAcquiredSemaphore);
    assert(res == VK_SUCCESS);

    // Get the index of the next available swapchain image:
    res = vkAcquireNextImageKHR(info.device, info.swap_chain, UINT64_MAX,
                                imageAcquiredSemaphore, VK_NULL_HANDLE,
                                &info.current_buffer);
    // TODO: Deal with the VK_SUBOPTIMAL_KHR and VK_ERROR_OUT_OF_DATE_KHR
    // return codes
    assert(res == VK_SUCCESS);

    set_image_layout(info, info.buffers[info.current_buffer].image,
                     VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED,
                     VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    VkRenderPassBeginInfo rp_begin;
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = info.render_pass;
    rp_begin.framebuffer = info.framebuffers[info.current_buffer];
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = info.width;
    rp_begin.renderArea.extent.height = info.height;
    rp_begin.clearValueCount = 2;
    rp_begin.pClearValues = clear_values;

    vkCmdBeginRenderPass(info.cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);
    vkCmdBindDescriptorSets(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            info.pipeline_layout, 0, NUM_DESCRIPTOR_SETS,
                            info.desc_set.data(), 0, NULL);

    const VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(info.cmd, 0, 1, &info.vertex_buffer.buf, offsets);

    init_viewports(info);
    init_scissors(info);

    //vkCmdDraw(info.cmd, 12 * 3, 1, 0, 0);
    vkCmdBindIndexBuffer(info.cmd,iBuffer,0,VK_INDEX_TYPE_UINT16);
    vkCmdDrawIndexed(info.cmd,36,1,0,0,0);
    vkCmdEndRenderPass(info.cmd);
    res = vkEndCommandBuffer(info.cmd);
    const VkCommandBuffer cmd_bufs[] = {info.cmd};
    VkFenceCreateInfo fenceInfo;
    VkFence drawFence;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
    vkCreateFence(info.device, &fenceInfo, NULL, &drawFence);

    VkPipelineStageFlags pipe_stage_flags =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit_info[1] = {};
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].waitSemaphoreCount = 1;
    submit_info[0].pWaitSemaphores = &imageAcquiredSemaphore;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;

    /* Queue the command buffer for execution */
    res = vkQueueSubmit(info.graphics_queue, 1, submit_info, drawFence);
    assert(res == VK_SUCCESS);

    /* Now present the image in the window */

    VkPresentInfoKHR present;
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pNext = NULL;
    present.swapchainCount = 1;
    present.pSwapchains = &info.swap_chain;
    present.pImageIndices = &info.current_buffer;
    present.pWaitSemaphores = NULL;
    present.waitSemaphoreCount = 0;
    present.pResults = NULL;

    /* Make sure command buffer is finished before presenting */
    do {
        res =
            vkWaitForFences(info.device, 1, &drawFence, VK_TRUE, FENCE_TIMEOUT);
    } while (res == VK_TIMEOUT);

    assert(res == VK_SUCCESS);
    res = vkQueuePresentKHR(info.present_queue, &present);
    assert(res == VK_SUCCESS);

    wait_seconds(1);
    /* VULKAN_KEY_END */
    if (info.save_images)
        write_ppm(info, "drawcube");

    vkDestroySemaphore(info.device, imageAcquiredSemaphore, NULL);
    vkDestroyFence(info.device, drawFence, NULL);
    destroy_pipeline(info);
    destroy_pipeline_cache(info);
    destroy_descriptor_pool(info);
    destroy_vertex_buffer(info);
    destroy_framebuffers(info);
    destroy_shaders(info);
    destroy_renderpass(info);
    destroy_descriptor_and_pipeline_layouts(info);
    destroy_uniform_buffer(info);
    destroy_depth_buffer(info);
    destroy_swap_chain(info);
    destroy_command_buffer(info);
    destroy_command_pool(info);
    destroy_device(info);
    destroy_window(info);
    destroy_instance(info);
    return 0;
}
