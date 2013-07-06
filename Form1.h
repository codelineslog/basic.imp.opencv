#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "cv.h"

namespace Grayscale {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	cv::Mat image, gray, thres;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox;
	protected: 

	protected: 
	private: System::Windows::Forms::Button^  btnOpen;
	private: System::Windows::Forms::Button^  btnGrayscale;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::Windows::Forms::Button^  btnThreshold;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->btnOpen = (gcnew System::Windows::Forms::Button());
			this->btnGrayscale = (gcnew System::Windows::Forms::Button());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->btnThreshold = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox
			// 
			this->pictureBox->Location = System::Drawing::Point(12, 10);
			this->pictureBox->Name = L"pictureBox";
			this->pictureBox->Size = System::Drawing::Size(320, 240);
			this->pictureBox->TabIndex = 0;
			this->pictureBox->TabStop = false;
			// 
			// btnOpen
			// 
			this->btnOpen->Location = System::Drawing::Point(338, 10);
			this->btnOpen->Name = L"btnOpen";
			this->btnOpen->Size = System::Drawing::Size(75, 23);
			this->btnOpen->TabIndex = 1;
			this->btnOpen->Text = L"Open";
			this->btnOpen->UseVisualStyleBackColor = true;
			this->btnOpen->Click += gcnew System::EventHandler(this, &Form1::btnOpen_Click);
			// 
			// btnGrayscale
			// 
			this->btnGrayscale->Location = System::Drawing::Point(338, 39);
			this->btnGrayscale->Name = L"btnGrayscale";
			this->btnGrayscale->Size = System::Drawing::Size(75, 23);
			this->btnGrayscale->TabIndex = 2;
			this->btnGrayscale->Text = L"Grayscale";
			this->btnGrayscale->UseVisualStyleBackColor = true;
			this->btnGrayscale->Click += gcnew System::EventHandler(this, &Form1::btnGrayscale_Click);
			// 
			// btnThreshold
			// 
			this->btnThreshold->Location = System::Drawing::Point(338, 68);
			this->btnThreshold->Name = L"btnThreshold";
			this->btnThreshold->Size = System::Drawing::Size(75, 23);
			this->btnThreshold->TabIndex = 3;
			this->btnThreshold->Text = L"Threshold";
			this->btnThreshold->UseVisualStyleBackColor = true;
			this->btnThreshold->Visible = false;
			this->btnThreshold->Click += gcnew System::EventHandler(this, &Form1::btnThreshold_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(422, 262);
			this->Controls->Add(this->btnThreshold);
			this->Controls->Add(this->btnGrayscale);
			this->Controls->Add(this->btnOpen);
			this->Controls->Add(this->pictureBox);
			this->Name = L"Form1";
			this->Text = L"Grayscale";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void btnOpen_Click(System::Object^  sender, System::EventArgs^  e) {
				 if ( openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
				 {
					 pictureBox->Load(openFileDialog->FileName);
					 System::String^ fileName = openFileDialog->FileName;
					 using namespace Runtime::InteropServices;
					 const char* chars = 
					 (const char*)(Marshal::StringToHGlobalAnsi(fileName)).ToPointer();
				     std::string imageFile = chars;
				     Marshal::FreeHGlobal(IntPtr((void*)chars));
					 image = cv::imread(imageFile, 1);
					 //http://msdn.microsoft.com/en-us/library/1b4az623(v=vs.80).aspx
				 }
			 }
	private: System::Void btnGrayscale_Click(System::Object^  sender, System::EventArgs^  e) {
				 gray = cv::Mat(image.cols, image.rows, CV_8UC1);
				 cv::cvtColor(image, gray, CV_BGR2GRAY);

				 cv::Mat mrg(image.cols, image.rows, CV_8UC3);

				 std::vector<cv::Mat> channels;
				 channels.push_back(gray);
				 channels.push_back(gray);
				 channels.push_back(gray);

				 merge(channels, mrg);

				DrawCvImage(pictureBox, mrg);				 
			 }

	private: System::Void btnThreshold_Click(System::Object^  sender, System::EventArgs^  e) {
				 cv:threshold(gray, thres, 128, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);

				 cv::Mat mrg(image.cols, image.rows, CV_8UC3);

				 std::vector<cv::Mat> channels;
				 channels.push_back(thres);
				 channels.push_back(thres);
				 channels.push_back(thres);

				 merge(channels, mrg);

				 DrawCvImage(pictureBox, mrg);
			}


	private: System::Void DrawCvImage(System::Windows::Forms::Control^ controls, cv::Mat& imgMat){
				 // convert cv::Mat image to Bitmap 
				 System::Drawing::Graphics^ graphics = controls->CreateGraphics();
				 System::Drawing::Bitmap^ b  = gcnew 
					 System::Drawing::Bitmap(imgMat.cols, imgMat.rows, imgMat.step,
					 System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) imgMat.data);
				 System::Drawing::RectangleF rect(0,0,controls->Width,controls->Height);
				 graphics->DrawImage(b,rect);
				 //http://stackoverflow.com/questions/12616529/displaying-webcam-feed-in-cvmat-format-in-a-picturebox/12628861#12628861
			 }
};
}

