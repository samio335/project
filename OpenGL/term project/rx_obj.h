/*! @file rx_obj.h
	
	@brief OBJ/MTL File Input/Output
 
	@author Makoto Fujisawa
	@date  
*/


#ifndef _RX_OBJ_H_
#define _RX_OBJ_H_


//-----------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------
#include <string>
#include <vector>

#include <map>

#include <iostream>
#include <fstream>
#include <sstream>

#include "rx_vec.h"


//-----------------------------------------------------------------------------
// Name Space
//-----------------------------------------------------------------------------
using namespace std;


#define RX_DEBUG_OUT 0

struct rxMaterialOBJ
{
	string name;

	float diffuse[4];
	float specular[4];
	float ambient[4];

	float color[4];
	float emission[4];

	double shininess;

	int illum;
	string tex_file;
	unsigned int tex_name;
};

struct rxFace
{
	vector<int> vertices;
	string material;
	vector<Vec2> texcoords;
};

typedef map<string, rxMaterialOBJ> rxMTL;


//-----------------------------------------------------------------------------
// rxOBJ�N���X�̐錾 - OBJ�`���̓ǂݍ���
//-----------------------------------------------------------------------------
class rxOBJ
{
	rxMTL m_mapMaterials;	//!< ���x���ƃf�[�^�̃}�b�v
	string m_strCurrentMat;				//!< ���݂̃f�[�^���������x��

	//vector<rxMaterialOBJ> m_vMaterials;
	//int m_iCurrentMat;

public:
	rxOBJ();
	~rxOBJ();

	bool Read(string file_name, vector<Vec3> &vrts, vector<Vec3> &vnms, vector<rxFace> &plys, rxMTL &mats, bool triangle = true);
	bool Save(string file_name, const vector<Vec3> &vrts, const vector<Vec3> &vnms, const vector<rxFace> &plys, const rxMTL &mats);

	rxMTL GetMaterials(void){ return m_mapMaterials; }

private:
	int loadFace(string &buf, vector<int> &vidxs, vector<int> &nidxs, vector<int> &tidxs);
	int loadMTL(const string &mtl_fn);
	int saveMTL(const string &mtl_fn, const rxMTL &mats);
};




/*!
 * ���_�񂩂��AABB�̌���
 * @param[out] minp,maxp AABB�̍ő���W�C�ŏ����W
 * @param[in] vec_set ���_��
 * @param[in] start_index,end_index ���_��̌����͈�
 * @return �����ł�����true
 */
inline bool FindBBox(Vec3 &minp, Vec3 &maxp, 
					 const vector<Vec3> &vec_set, 
					 const int start_index, const int end_index)
{
	if((int)vec_set.size() == 0) return false;

	maxp = vec_set[start_index];
	minp = vec_set[start_index];

	for(int i = start_index+1; i < end_index; ++i){
		if(vec_set[i][0] > maxp[0]) maxp[0] = vec_set[i][0];
		if(vec_set[i][1] > maxp[1]) maxp[1] = vec_set[i][1];
		if(vec_set[i][2] > maxp[2]) maxp[2] = vec_set[i][2];
		if(vec_set[i][0] < minp[0]) minp[0] = vec_set[i][0];
		if(vec_set[i][1] < minp[1]) minp[1] = vec_set[i][1];
		if(vec_set[i][2] < minp[2]) minp[2] = vec_set[i][2];
	}

	return true;
}
/*!
 * ���_�񂩂��AABB�̌���
 * @param[out] minp,maxp AABB�̍ő���W�C�ŏ����W
 * @param[in] vec_set ���_��
 * @return �����ł�����true
 */
inline bool FindBBox(Vec3 &minp, Vec3 &maxp, const vector<Vec3> &vec_set)
{
	return FindBBox(minp, maxp, vec_set, 0, (int)vec_set.size());
}

/*!
 * ���_���AABB�ɍ����悤��Fit������
 * @param[in] ctr AABB���S���W
 * @param[in] sl  AABB�̕ӂ̒���(1/2)
 * @param[in] vec_set ���_��
 * @param[in] start_index,end_index ���_��̌����͈�
 */
inline bool FitVertices(const Vec3 &ctr, const Vec3 &sl, 
						vector<Vec3> &vec_set, 
						const int start_index, const int end_index)
{
	Vec3 ctr0, sl0, maxp, minp;

	// ���݂�BBox�̑傫���𒲂ׂ�
	FindBBox(minp, maxp, vec_set, start_index, end_index);
			
	sl0  = (maxp-minp)/2.0;
	ctr0 = (maxp+minp)/2.0;

	int max_axis = ( ( (sl0[0] > sl0[1]) && (sl0[0] > sl0[2]) ) ? 0 : ( (sl0[1] > sl0[2]) ? 1 : 2 ) );
	int min_axis = ( ( (sl0[0] < sl0[1]) && (sl0[0] < sl0[2]) ) ? 0 : ( (sl0[1] < sl0[2]) ? 1 : 2 ) );
	double size_conv = sl[max_axis]/sl0[max_axis];

	// �S�Ă̒��_��bbox�ɂ��킹�ĕϊ�
	for(int i = start_index; i < end_index; ++i){
		vec_set[i] = (vec_set[i]-ctr0)*size_conv+ctr;
	}

	return true;
}

/*!
 * ���_���AABB�ɍ����悤��Fit������
 * @param[in] ctr AABB���S���W
 * @param[in] sl  AABB�̕ӂ̒���(1/2)
 * @param[in] vec_set ���_��
 */
inline bool FitVertices(const Vec3 &ctr, const Vec3 &sl, vector<Vec3> &vec_set)
{
	FitVertices(ctr, sl, vec_set, 0, (int)vec_set.size());
	return true;
}


#endif // _RX_VRML_H_
