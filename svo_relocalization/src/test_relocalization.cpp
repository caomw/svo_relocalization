
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Eigen/Core>
#include <sophus/se3.h>
#include <svo_relocalization/klein_murray_relocalizter.h>

using namespace std;
using namespace Eigen;
using namespace reloc;


void readMatrixFromFile (vector<cv::Mat>& images, vector<Sophus::SE3>& poses, vector<int>& ids)
{
  std::ifstream file_in("/home/fox/catkin_ws/src/svo_relocalization/test_data/pose.txt");

  string path ("/home/fox/catkin_ws/src/svo_relocalization/test_data/");
  string line;
  int id=0;
  float x, y, z;
  float rw, rx, ry, rz;

  //while (!file_in.eof())
  //{
    file_in >> id >> x >> y >> z >> rw >> rx >> ry >> rz;
    
    stringstream filename;
    filename << path << "image" << id << ".png";
    cv::Mat im;
    im = cv::imread(filename.str(), CV_LOAD_IMAGE_GRAYSCALE);
    images.push_back(im);

    Vector3d t(x,y,z);
    Eigen::Quaterniond q(rw, rx, ry, rz);
    poses.push_back(Sophus::SE3(q, t));

    ids.push_back(id);
  //}
}

int main(int argc, char const *argv[])
{
  
  vector<cv::Mat> images;
  vector<Sophus::SE3> poses;
  vector<int> ids;

  readMatrixFromFile(images, poses, ids);
  cout << images.size() << endl;
  cout << "Image size: " << images[0].size() << endl;


  KMRelocalizer relocalizer;

  for (size_t i = 0; i < 1; i+=10)
  {
    vector<cv::Mat> tmp_vector;
    tmp_vector.push_back(images.at(i));
    relocalizer.addFrame(tmp_vector, poses[i], ids[i]);
  }

  int query_idx = 0;
  vector<cv::Mat> tmp_vector;
  tmp_vector.push_back(images.at(query_idx));
  Sophus::SE3 T_f_out;
  int id_out;

  relocalizer.relocalize(tmp_vector, poses.at(query_idx), T_f_out, id_out);


  return 0;
}