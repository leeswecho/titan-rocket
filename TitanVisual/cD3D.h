#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "common_classes.h"
//
#define MAX_D3D_DEVICES		10

//defines the size of the history buffer
#define HISTORY_BUFFER_SIZE 200000

#define ORIENTATION_MODE_NONE	       0x00
#define ORIENTATION_MODE_LATLONGPR    0x01
#define ORIENTATION_MODE_PITCHROLLYAW  0x02
#define ORIENTATION_MODE_BODYXYZ		0x04

const DWORD vertex_fvf=D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_DIFFUSE;
const DWORD vertex_2d=D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

//this is a point in the history
class cHistoryPoint
{
public:
	double px;
	double py; 
	double pz;
	double longitude;
	double latitude;
	double radius;
	bool bValid; //whether this point is current filled with valid data

	//default constructor
	cHistoryPoint()
	{
		px = 0; py = 0; pz = 0; longitude = 0; latitude = 0; radius = 0; bValid = false;

	}
private:

};

//this is a buffer that stores previous location history of the object in question.
class cHistoryBuffer
{
public:
	//for now, save 10,000 points
	cHistoryPoint Points[HISTORY_BUFFER_SIZE];
	//this is the corresponding "pre" vertex buffer that will hold the vertex data that will actually be
	//passed to the renderer. unfortunately this buffer has to be constructed every time, due to the changing
	//position offsets.
	vertex Vertexes[HISTORY_BUFFER_SIZE];
	//this is the actual vertex buffer	
	IDirect3DVertexBuffer9* VertexBuffer;
	//material which will define the line color
	D3DMATERIAL9 Material;

	//stores whether to draw the history for this buffer right now. 
	bool bEnable;
	//the current "youngest" index (the one to draw first)
	int curr_index;

	//clear the history buffer
	int clear()
	{
		//go through all points, but don't bother clearing the values,
		//just set valid to 0
		for (int i = 0; i < HISTORY_BUFFER_SIZE; i++)
			Points[i].bValid = false;
		//reset current index to 0
		curr_index = 0;

		return 0;
	}

	//default constructor
	cHistoryBuffer()
	{
		curr_index = 0; bEnable = false;
	}
	//get last history point
	VECTOR3 get_last_point()
	{
		int index = curr_index - 1;
		//wrap
		if (index < 0)
			index += HISTORY_BUFFER_SIZE;

		//if enabled
		if (Points[index].bValid == false)
			return VECTOR3(0,0,0);

		//else use this
		return VECTOR3(Points[index].px, Points[index].py, Points[index].pz);
	}
	//add a point to the history
	int add_point(double px, double py, double pz)
	{
		//populate current index
		Points[curr_index].px = px;
		Points[curr_index].py = py;		
		Points[curr_index].pz = pz;
		//enable
		Points[curr_index].bValid = true;
		//increment pointer
		curr_index++;
		//wrap if necessary
		if (curr_index >= HISTORY_BUFFER_SIZE)
			curr_index -= HISTORY_BUFFER_SIZE;
		//done.
		return 0;
	}

private:

};


class cRenderObject
{
public:

	char Name[80];
	int set_name(char* _Name);

	//this is the "pre" vertex buffer if we're 3D
	vertex* Vertexes; //note: i'm not sure if this is actually used at the moment.
	// vertex buffer if we're 2D
	//note that it is a maximum of 4 vertices
	vertex2d Vertexes2d[4];
	//this is a flag to use fixed scale when rendering this object in 2D
	bool bFixedScale2D;
	//this is the number of vertices in either buffer
	int iNumVertexes;

	IDirect3DVertexBuffer9* VertexBuffer;
	IDirect3DTexture9* Texture;
	D3DMATERIAL9 Material;

	int iOrientationMode;
	double latitude;
	double longitude;
	double pr;
	double pitch;
	double roll;
	double yaw;
	double px;
	double py;
	double pz;

	//this saves the "absolute" position of the object when it is calculated
	VECTOR3 abs_position;

	//this is an optional "history buffer" that contains 
	cHistoryBuffer* pHistoryBuffer;

	int init();
	int exit();
	
	cRenderObject() {  init(); }

	cRenderObject* add_child();

	cRenderObject*	pFirstChild;
	cRenderObject*	pNextSibling;


private:

};

class cD3D_Device
{
public:

	FILE* dfile;

	IDirect3DDevice9* Device;
	D3DPRESENT_PARAMETERS DeviceParams;
	int iDeviceID;
	cRenderObject*	RenderList;

	ID3DXFont *m_font;
	ID3DXFont *m_font_large;
	ID3DXFont *m_font_small;

	D3DXVECTOR3 eye_vector;
	D3DXVECTOR3 lookat_vector;
	D3DXVECTOR3 up_vector;
	bool bViewChanged;
	double cam_yaw;
	double cam_tilt;
	double cam_dist;
	D3DXVECTOR3 calc_lookat();
	D3DXVECTOR3 calc_eye();
	bool bFollow;

	D3DLIGHT9 light0;
	bool bLight0Changed;

	D3DLIGHT9 light1;
	int set_lights();

	int render_text(double sim_time);
	int render_text_2d();

	int set_z_planes(double zNear, double zFar);

	int render_count_0;
	int render_count_1;

	int build_globe(cRenderObject* pObject);

	int iHeight;
	int iWidth;

	//this is used to save the original position offsets that the render call was started with
	VECTOR3	save_start_offset; 

	//the "main" render function
	int render_object(cRenderObject* object_to_render, MATRIX BaseTransform);
	//the "main" render function for a 2D window
	int render_object_2d(cRenderObject* object_to_render, MATRIX BaseTransform, double input_scale);
	//this renders a history line using the supplied history buffer
	int render_history_line(cHistoryBuffer* pHistBuff, VECTOR3 position_offset, MATRIX CurrTransform);
	//the 2d version
	int render_history_line_2d(cHistoryBuffer* pHistBuff, double x_offset, double y_offset, double input_scale);

	MATRIX apply_body_translation(MATRIX input, double px, double py, double pz);
	MATRIX apply_body_latlongpr(MATRIX input, double latitude, double longitude, double pr);
	MATRIX apply_body_pitchrollyaw(MATRIX input, double pitch, double roll, double yaw);
	
	D3DMATRIX GetD3DMatrix(MATRIX input);

	int reverse_normals(cRenderObject* pObj);

	int init(int _iDeviceID);
	int exit();
	int render0(VECTOR3 start_offset, double sim_time);
	int render1(VECTOR3 start_offset, double input_scale, double sim_time);
	int init_views0();
	int init_views1();

	int set_views();

	//initializes a render object using an array of 3D vertices, and a texture file
	int load_with_model(cRenderObject* input_object, vertex* buffer, int _iNumVertexes, WCHAR* TextureFile);
	//adds a history buffer to a render object 
	int add_history_buffer(cRenderObject* input_object, bool initial_enable_status, D3DXCOLOR history_line_color); 
	//the 2D version
	int add_history_buffer_2d(cRenderObject* input_object, bool initial_enable_status, D3DXCOLOR history_line_color); 
	//initializes a render object as a 2D sprite (a polygon and a texture file)
	int load_with_sprite(cRenderObject* input_object, vertex2d* buffer, int _iNumVertexes, WCHAR* TextureFile);
	
	int init_vertex_buffer(cRenderObject* pObject, int _iNumVertexes);
	//2d version
	int init_vertex_buffer_2d(cRenderObject* pObject, int _iNumVertexes);

	//add one for the history
	int init_vertex_buffer_for_history(cHistoryBuffer* pHistBuff, int _iNumVertexes);
	//2D version
	int init_vertex_buffer_for_history_2d(cHistoryBuffer* pHistBuff, int _iNumVertexes);

	//2d version
	int update_vertex_buffer_2d(cRenderObject* input_object, double x_offset, double y_offset, double input_scale);
	//alternate 2d version, assumes that rendered object fills the screen
	int update_vertex_buffer_2d_alt(cRenderObject* input_object, double x_offset, double y_offset, double input_scale);

	int eye_tilt_up();
	int eye_tilt_down();
	int eye_tilt_left();
	int eye_tilt_right();
	int eye_forward();
	int eye_back();
	int pan_left();
	int pan_right();
	int pan_up();
	int pan_down();
	int zoom_in();
	int zoom_out();

	
	int follow_on();
	int follow_off();

	int init_lights0();
	int init_lights1();

	//initialize a renderstate for 3D viewing
	int init_renderstate_3d();
	//initialize a renderstate for 2D viewing
	int init_renderstate_2d();


	//DEBUG
	int debug_write();

private:

};

class cD3D
{
public:
	IDirect3D9* D3D;
	
	int init();
	int exit();

	int load_window_3d(int iWindow, HWND _hWindow, int _iWidth, int _iHeight);
	int load_window_2d(int iWindow, HWND _hWindow, int _iWidth, int _iHeight);

	int render(VECTOR3 start_offset0, VECTOR3 start_offset1, double scale1, double sim_time);

	cD3D_Device Devices[MAX_D3D_DEVICES];


private:

};