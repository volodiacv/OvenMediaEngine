//==============================================================================
//
//  Transcode
//
//  Created by Kwon Keuk Han
//  Copyright (c) 2018 AirenSoft. All rights reserved.
//
//==============================================================================

#pragma once

#include "media_filter_impl.h"
#include "base/media_route/media_buffer.h"
#include "base/media_route/media_type.h"

#include "../transcode_context.h"

class MediaFilterRescaler : public MediaFilterImpl
{
public:
	MediaFilterRescaler();
	~MediaFilterRescaler();

	bool Configure(const std::shared_ptr<MediaTrack> &input_media_track, const std::shared_ptr<TranscodeContext> &input_context, const std::shared_ptr<TranscodeContext> &output_context) override;

	int32_t SendBuffer(std::shared_ptr<MediaFrame> buffer) override;
	std::shared_ptr<MediaFrame> RecvBuffer(TranscodeResult * result) override;

	void ThreadEncode();

	void Stop();

private:
	AVFrame *_frame = nullptr;
	AVFilterContext *_buffersink_ctx = nullptr;
	AVFilterContext *_buffersrc_ctx = nullptr;
	AVFilterGraph *_filter_graph = nullptr;
	AVFilterInOut *_inputs = nullptr;
	AVFilterInOut *_outputs = nullptr;

	double _scale = 0.0;

	std::deque<std::shared_ptr<MediaFrame>> _input_buffer;
	std::deque<std::shared_ptr<MediaFrame>> _output_buffer;

	std::shared_ptr<TranscodeContext> _input_context;
	std::shared_ptr<TranscodeContext> _output_context;

	bool _kill_flag = false;
	std::mutex _mutex;
	std::thread _thread_work;
	ov::Semaphore _queue_event;
};
