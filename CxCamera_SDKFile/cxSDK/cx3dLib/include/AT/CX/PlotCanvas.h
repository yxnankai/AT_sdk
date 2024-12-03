/*************************************************************************************
@brief Implementation of a simple plot class based on OpenCV drawing functions
@file : PlotCanvas.h
@package : cx_3d library

@copyright (c) 2017, Automation Technology GmbH.

@version 01.09.2017 initial version

*************************************************************************************/
#pragma once
#ifndef CX_PLOTCANVAS_INCLUDED
#define CX_PLOTCANVAS_INCLUDED

#include "opencv2/opencv.hpp"

/// Namespace for CX library objects 
namespace AT {
namespace cx {

class PlotCanvas
{
public:
	PlotCanvas(int height=500, int width=500)
	{
		m_csz.width = width;
		m_csz.height = height;
		setArea(cv::Rect2f(0.0, 0.0, float(m_csz.width), float(m_csz.height)));
		m_l2c_o.x = m_l2c_o.y = 0.0;
		clear();
	}

	void clear()
	{
		m_canvas.create(m_csz.height, m_csz.width, CV_8UC3);
		m_canvas = cv::Scalar(255, 255, 255);	// fill with background color 
	}

	void setArea(cv::Rect2f prect)
	{
		m_lArea = prect;
		m_l2c_s.x = float(m_csz.width) / prect.width;
		m_l2c_s.y = float(m_csz.height) / prect.height;
		m_l2c_o = m_lArea.tl();
	}

	cv::Point2i l2c(const cv::Point2f& l)
	{
		cv::Point2i p;
		p.x = (int)((l.x - m_l2c_o.x)*m_l2c_s.x + 0.5f);
		p.y = (int)((l.y - m_l2c_o.y)*m_l2c_s.y + 0.5f);
		// make it inside canvas
		if (p.x < 0) p.x = 0;
		if (p.x >= m_csz.width) p.x = m_csz.width-1;
		if (p.y < 0) p.y = 0;
		if (p.y >= m_csz.height) p.y = m_csz.height - 1;
		return p;
	}
	
	void plotVector(cv::Point2f p0, cv::Point2f d, cv::Scalar color)
	{
		cv::Point2i point2ip0 = l2c(p0);
		cv::Point2i point2ip0pd = l2c(p0 + d);
		cv::line(m_canvas, point2ip0, point2ip0pd, color);
	}

	void plotMarker(cv::Point2f p, cv::Scalar color, int markerType)
	{
		cv::Point2i point2ip = l2c(p);
		cv::drawMarker(m_canvas, point2ip, color, markerType, 5, 1, 8);
	}

	void text(cv::Point2f p, const std::string& txt, cv::Scalar color, int fontFace=cv::FONT_HERSHEY_PLAIN, double fontScale=1.0)
	{
		cv::Point2i point2ip = l2c(p);
		cv::putText(m_canvas, txt, point2ip, fontFace, fontScale, color);
	}

	void show(std::string name)
	{
		cv::imshow(name, m_canvas);
	}

protected:
	cv::Rect2f m_lArea;		//!< area in logical units
	cv::Point2f m_l2c_o;	//!< offset logic to canvas pixel
	cv::Point2f m_l2c_s;	//!< scaling factor logic to canvas pixel
	cv::Size2i m_csz;		//!< canvas size in pixeln
	cv::Mat m_canvas;
};

}}	// namespace AT::CX
#endif // CX_PLOTCANVAS_INCLUDED
