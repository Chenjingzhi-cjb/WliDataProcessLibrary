#ifndef WLI_DATA_ALGORITHM_HPP
#define WLI_DATA_ALGORITHM_HPP

#include "wli_data_reader.hpp"

#include <stdexcept>
#include <numeric>

//#include "opencv2/opencv.hpp"

#include "pcl/point_types.h"
#include "pcl/point_cloud.h"
#include "pcl/io/ply_io.h"
#include "pcl/io/pcd_io.h"
#include "pcl/visualization/cloud_viewer.h"


class WliDataAlgorithm {
private:
    WliDataAlgorithm() = default;

    ~WliDataAlgorithm() = default;

public:
    /**
     * @brief 将 WLI data 转为 PCL Point Cloud 对象
     *
     * @param reader The WLI data reader.
     * @return point cloud shared ptr
     */
    static pcl::PointCloud<pcl::PointXYZ>::Ptr wliDataToPointCloud(WliDataReader &reader) {
        auto data = reader.getData();

        pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud(new pcl::PointCloud<pcl::PointXYZ>);

        for (auto &n : data) {
            pcl::PointXYZ point;
            point.x = (float) n[0];
            point.y = (float) n[1];
            point.z = (float) n[2];

            point_cloud->push_back(point);
        }

        return point_cloud;
    }

    /**
     * @brief 将 WLI data 保存为 PLY 点云文件
     *
     * @param reader The WLI data reader.
     * @param file_path The file path to save the WLI data. (end with ".ply")
     * @param type The type of store data. 0 for binary, 1 for ascii.
     * @return None
     */
    static void saveWliDataToPly(WliDataReader &reader, const std::string &file_path, int type = 0) {
        if (file_path.substr(file_path.find_last_of('.') + 1) != "ply") {
            std::cout << "saveWliDataToPly() Error: File path must end with '.ply'." << std::endl;
            return;
        }

        auto cloud = wliDataToPointCloud(reader);

        if (type == 0) {
            pcl::io::savePLYFileBinary(file_path, *cloud);
        } else if (type == 1) {
            pcl::io::savePLYFileASCII(file_path, *cloud);
        }
    }

    /**
     * @brief 将 WLI data 保存为 PCD 点云文件
     *
     * @param reader The WLI data reader.
     * @param file_path The file path to save the WLI data. (end with ".pcd")
     * @param type The type of store data. 0 for binary, 1 for ascii.
     * @return None
     */
    static void saveWliDataToPcd(WliDataReader &reader, const std::string &file_path, int type = 0) {
        if (file_path.substr(file_path.find_last_of('.') + 1) != "pcd") {
            std::cout << "saveWliDataToPcd() Error: File path must end with '.pcd'." << std::endl;
            return;
        }

        auto cloud = wliDataToPointCloud(reader);

        if (type == 0) {
            pcl::io::savePCDFileBinary(file_path, *cloud);
        } else if (type == 1) {
            pcl::io::savePCDFileASCII(file_path, *cloud);
        }
    }
};


#endif //WLI_DATA_ALGORITHM_HPP
