/*
 * util.h
 *
 *  Created on: Jul 4, 2013
 *      Author: vsu
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/PolygonMesh.h>

#include <octomap/OcTree.h>
#include <octomap/ColorOcTree.h>

typedef struct {
	int cam_id;
	int point_id;
	Eigen::Vector2f coord;
} observation;

class keypoint_map {

public:

	keypoint_map(cv::Mat & rgb, cv::Mat & depth, Eigen::Affine3f & transform);
	keypoint_map(const std::string & dir_name);

	bool merge_keypoint_map(const keypoint_map & other);

	void remove_bad_points();

	void optimize();

	void get_octree(octomap::OcTree & tree);

	void extract_surface();

	float compute_error();

	void save(const std::string & dir_name);

	cv::Ptr<cv::FeatureDetector> fd;
	cv::Ptr<cv::DescriptorExtractor> de;
	cv::Ptr<cv::DescriptorMatcher> dm;

	pcl::PointCloud<pcl::PointXYZ> keypoints3d;
	cv::Mat descriptors;
	std::vector<float> weights;

	std::vector<Eigen::Affine3f> camera_positions;
	std::vector<observation> observations;

	Eigen::Vector4f intrinsics;

	std::vector<cv::Mat> rgb_imgs;
	std::vector<cv::Mat> depth_imgs;

};

void compute_features(const cv::Mat & rgb, const cv::Mat & depth,
		const Eigen::Vector4f & intrinsics, cv::Ptr<cv::FeatureDetector> & fd,
		cv::Ptr<cv::DescriptorExtractor> & de,
		std::vector<cv::KeyPoint> & filtered_keypoints,
		pcl::PointCloud<pcl::PointXYZ> & keypoints3d, cv::Mat & descriptors);

bool estimate_transform_ransac(const pcl::PointCloud<pcl::PointXYZ> & src,
		const pcl::PointCloud<pcl::PointXYZ> & dst,
		const std::vector<cv::DMatch> matches, int num_iter,
		float distance2_threshold, int min_num_inliers, Eigen::Affine3f & trans,
		std::vector<bool> & inliers);

#endif /* UTIL_H_ */