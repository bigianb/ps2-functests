
#include "vu1_test.h"

#include <tamtypes.h>
#include <dma.h>
#include <packet2.h>
#include <packet2_utils.h>

extern u32 vu1_code_start __attribute__((section(".vudata")));
extern u32 vu1_code_end __attribute__((section(".vudata")));

void vu1_upload_micro_program()
{
	// + 1 for end tag
	u32 packet_size = packet2_utils_get_packet_size_for_program(&vu1_code_start, &vu1_code_end) + 1; 
	packet2_t *packet2 = packet2_create(packet_size, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	packet2_vif_add_micro_program(packet2, 0, &vu1_code_start, &vu1_code_end);
	packet2_utils_vu_add_end_tag(packet2);
	dma_channel_send_packet2(packet2, DMA_CHANNEL_VIF1, 1);
	dma_channel_wait(DMA_CHANNEL_VIF1, 0);
	packet2_free(packet2);
}

bool VU1Test::run()
{
    dma_channel_initialize(DMA_CHANNEL_VIF1, NULL, 0);
	dma_channel_fast_waits(DMA_CHANNEL_VIF1);
    vu1_upload_micro_program();

	packet2_t* curr_vif_packet = packet2_create(11, P2_TYPE_NORMAL, P2_MODE_CHAIN, 1);
	packet2_reset(curr_vif_packet, 0);
	packet2_utils_vu_add_start_program(curr_vif_packet, 0);
	packet2_utils_vu_add_end_tag(curr_vif_packet);
	dma_channel_wait(DMA_CHANNEL_VIF1, 0);
	dma_channel_send_packet2(curr_vif_packet, DMA_CHANNEL_VIF1, 1);

	// need to wait for VU prog to finish.
	// This isn't the way to do it.
	dma_channel_wait(DMA_CHANNEL_VIF1, 0);

	packet2_free(curr_vif_packet);
	return true;
}
