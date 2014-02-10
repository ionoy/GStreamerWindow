#pragma once
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

namespace GStreamer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	public ref class VideoWindow : public System::Windows::Forms::Form
	{
	public:
		VideoWindow(System::String^ pipelineString)
		{
			InitializeComponent();
			
			IntPtr ptr = Marshal::StringToHGlobalAnsi(pipelineString);
			
			gst_init(NULL, NULL);

			_handle = Handle.ToInt32();
			const char* ppl = (const gchar*)ptr.ToPointer();
			_pipeline = gst_parse_launch(ppl, NULL);
			
			gst_element_set_state(_pipeline, GST_STATE_READY);

			GstElement* video_sink = gst_bin_get_by_interface(GST_BIN(_pipeline), GST_TYPE_X_OVERLAY);

			if (!video_sink) {
				GST_ERROR("Could not retrieve video sink");
				return;
			}

			gst_x_overlay_set_window_handle(GST_X_OVERLAY(video_sink), _handle);

			gst_element_set_state(_pipeline, GST_STATE_PLAYING);

			Marshal::FreeHGlobal(ptr);			
		}

	protected:
		~VideoWindow()
		{
			gst_element_set_state(_pipeline, GST_STATE_NULL);
			gst_object_unref(_pipeline);

			if (components)
			{
				delete components;
			}
		}

	private:
		static guintptr _handle;
		GstElement *_pipeline;
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->Size = System::Drawing::Size(300,300);
			this->Text = L"VideoWindow";
			this->Padding = System::Windows::Forms::Padding(0);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		}
#pragma endregion
	};
}
