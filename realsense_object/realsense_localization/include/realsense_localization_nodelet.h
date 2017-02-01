/******************************************************************************
ClassifyImage	INTEL CORPORATION PROPRIETARY INFORMATION
	This software is supplied under the terms of a license agreement or nondisclosure
	agreement with Intel Corporation and may not be copied or disclosed except in
	accordance with the terms of that agreement
	Copyright(c) 2011-2015 Intel Corporation. All Rights Reserved.
*******************************************************************************/

#pragma once
# ifndef RS_LOCALIZATION_NODELET
# define RS_LOCALIZATION_NODELET

///////////////////////////////////////////////
/// Dependencies
///////////////////////////////////////////////

#include <nodelet/nodelet.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>

#include <librealsense/rs.hpp>
#include "utils.h"


namespace realsense
{
	///////////////////////////////////////////////
	///	CLocalization - 
	///////////////////////////////////////////////
	class CLocalization 
	{
	public :
		//===================================
		//	Interface
		//===================================
	  	void getParams(ros::NodeHandle& nh);
		void getParams(const std::vector<std::string> & argv);
		void init(ros::NodeHandle& nh);
		
		CLocalization();
		virtual ~CLocalization();

	private:
		//===================================
		//	Member Functions
		//===================================

		int initialize();
                int unInitialize();

                //Static member functions:
		void colorCameraImfoCallback(const sensor_msgs::CameraInfo::ConstPtr & cameraInfo);
		void depthCameraImfoCallback(const sensor_msgs::CameraInfo::ConstPtr & cameraInfo);
		void draw_results(rs::object_recognition::localization_data* localization_data,int array_size,rs::object_recognition::or_configuration_interface* or_configuration);
		
		void LocalizationCallback(const sensor_msgs::ImageConstPtr& color ,const sensor_msgs::ImageConstPtr& depth );

		
		//===================================
		//	Member Variables
		//===================================

		image_transport::Subscriber m_sub_color;
		image_transport::Subscriber m_sub_depth;			
		ros::Subscriber m_sub_objects_with_pos;	
		ros::Subscriber m_sub_colorCameraInfo;
		ros::Subscriber m_sub_depthCameraInfo;
		
		std::unique_ptr<message_filters::Subscriber<sensor_msgs::Image>> 			mDepthSubscriber;
		std::unique_ptr<message_filters::Subscriber<sensor_msgs::Image>>			mColorSubscriber;
		std::unique_ptr<message_filters::TimeSynchronizer<sensor_msgs::Image, sensor_msgs::Image>> mTimeSynchronizer;
			
		ros::Publisher m_recognized_objects_pub;
		ros::Publisher m_cpu_gpu_pub;
			
		cv::Mat m_imageColor;
		cv::Mat m_imageDepth;

		
		bool m_show_rgb;
		bool m_estimateCenter;
		float m_confidence;
		int m_maxNumberOfObjects;
		bool m_use_CPU;
		bool m_no_subscribers;

		ros::NodeHandle m_nh;
		
		int m_colorHeight;
		int m_colorWidth;
		int m_depthHeight;
		int m_depthWidth;
		
		rs::extrinsics m_ext;
		rs::intrinsics m_colorInt;
		rs::intrinsics m_depthInt;	
		
		ros_camera_utils utils;
		
		rs::core::image_info m_colorInfo;
		rs::core::image_info m_depthInfo;
		
		rs::object_recognition::or_video_module_impl m_impl;
		rs::object_recognition::or_data_interface* m_or_data; 
		rs::object_recognition::or_configuration_interface* m_or_configuration;		
	};
	
#ifdef RUN_AS_NODELET
	///////////////////////////////////////////////
	///	CLocalizationNodelet 
	///////////////////////////////////////////////
	class CLocalizationNodelet : public nodelet::Nodelet
	{
	public :
		//===================================
		//	Interface
		//===================================
		virtual void onInit();

		~CLocalizationNodelet();

	private:
		//===================================
		//	Member Functions
		//===================================

		//===================================
		//	Member Variables
		//===================================

		CLocalization m_RecgonitionNodelet;

	};
#endif
};


#endif // RS_RECOGNITION_NODELET
