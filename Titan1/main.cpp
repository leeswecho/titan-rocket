//Description: Titan1 was a testbed for all the various Direct3D
//rendering features that would be needed for the actual visualizer.
//Titan1 was used to test what would be needed to render lighted,
//textured polygons in Direct3D windowed mode. 
#define IDI_ICON1 101
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

WNDCLASS wndclass1;
HWND hMainWnd;
HINSTANCE hAppInstance;
bool bRunFlag;

IDirect3D9 *g_D3D=NULL;
D3DPRESENT_PARAMETERS DeviceParams;
IDirect3DDevice9* g_d3d_device = NULL;
IDirect3DVertexBuffer9* g_vertex_buffer = NULL;
IDirect3DTexture9* g_texture = NULL;
struct vertex{
   float x, y, z;
   float nx, ny, nz;
   DWORD colour;
   float tu, tv;
};
const DWORD vertex_fvf=D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_DIFFUSE;

struct vertex2{
   float x, y, z;
   float nx, ny, nz;
   float tu, tv;
};

const DWORD vertex_fvf2 = D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_NORMAL ;


double angle = 0;

LRESULT CALLBACK WndCallBackProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//**************************************************************************************//
//**************************************************************************************//
int create_window()
{
	wndclass1.lpszClassName = L"WindowClass1";
	wndclass1.hInstance = hAppInstance;
	wndclass1.hCursor = LoadCursor(hAppInstance, (LPCTSTR)IDC_ARROW);
	wndclass1.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass1.lpfnWndProc = WndCallBackProc;
	wndclass1.lpszMenuName = NULL;
	wndclass1.style = CS_DBLCLKS;
	wndclass1.hIcon = LoadIcon(hAppInstance, (LPCTSTR)IDI_ICON1);
	wndclass1.cbClsExtra = 0;
	wndclass1.cbWndExtra = 0;

	RegisterClass(&wndclass1);

	hMainWnd = CreateWindow(L"WindowClass1",
						L"Main Application Window",
						WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
						0,
						0,
						640,
						480,
						NULL,
						NULL,
						hAppInstance,
						NULL);

	return 0;

}
//**************************************************************************************//
//**************************************************************************************//
D3DMATRIX *dhMatrixTranslation(D3DMATRIX *p_out,float p_x,float p_y,float p_z){

   p_out->_11 = 1.0f;    p_out->_12 = 0.0f;    p_out->_13 = 0.0f;    p_out->_14 = 0.0f;
   p_out->_21 = 0.0f;    p_out->_22 = 1.0f;    p_out->_23 = 0.0f;    p_out->_24 = 0.0f;
   p_out->_31 = 0.0f;    p_out->_32 = 0.0f;    p_out->_33 = 1.0f;    p_out->_34 = 0.0f;
   p_out->_41 = p_x;     p_out->_42 = p_y;     p_out->_43 = p_z;     p_out->_44 = 1.0f;

   return p_out;
}
//**************************************************************************************//
//**************************************************************************************//
D3DMATRIX *dhMatrixRotationX(D3DMATRIX *p_out, float p_angle ){
float my_sin, my_cos;


   my_sin=(float)sin(p_angle);
   my_cos=(float)cos(p_angle);

   p_out->_11 = 1.0f; p_out->_12 =  0.0f;   p_out->_13 = 0.0f;    p_out->_14 = 0.0f;
   p_out->_21 = 0.0f; p_out->_22 =  my_cos; p_out->_23 = my_sin;  p_out->_24 = 0.0f;
   p_out->_31 = 0.0f; p_out->_32 = -my_sin; p_out->_33 = my_cos;  p_out->_34 = 0.0f;
   p_out->_41 = 0.0f; p_out->_42 =  0.0f;   p_out->_43 = 0.0f;    p_out->_44 = 1.0f;

   return p_out;
}
//**************************************************************************************//
//**************************************************************************************//
D3DMATRIX *dhMatrixRotationY(D3DMATRIX *p_out, float p_angle ){
float my_sin, my_cos;


   my_sin=(float)sin(p_angle);
   my_cos=(float)cos(p_angle);

   p_out->_11 =  my_cos;  p_out->_12 =  0.0f;   p_out->_13 = my_sin;    p_out->_14 = 0.0f;
   p_out->_21 = 0.0f;     p_out->_22 = 1.0f;    p_out->_23 = 0.0f;      p_out->_24 = 0.0f;
   p_out->_31 = -my_sin;  p_out->_32 = 0.0f;    p_out->_33 = my_cos;    p_out->_34 = 0.0f;
   p_out->_41 = 0.0f;     p_out->_42 =  0.0f;   p_out->_43 = 0.0f;      p_out->_44 = 1.0f;

   return p_out;
}
//**************************************************************************************//
//**************************************************************************************//
D3DMATRIX *dhMatrixRotationZ(D3DMATRIX *p_out, float p_angle ){
float my_sin, my_cos;


   my_sin=(float)sin(p_angle);
   my_cos=(float)cos(p_angle);

   p_out->_11 =  my_cos;  p_out->_12 = my_sin;  p_out->_13 =  0.0f;     p_out->_14 = 0.0f;
   p_out->_21 = -my_sin;  p_out->_22 = my_cos;  p_out->_23 = 0.0f;      p_out->_24 = 0.0f;
   p_out->_31 = 0.0f;     p_out->_32 = 0.0f;    p_out->_33 = 1.0f;      p_out->_34 = 0.0f;
   p_out->_41 = 0.0f;     p_out->_42 =  0.0f;   p_out->_43 = 0.0f;      p_out->_44 = 1.0f;

   return p_out;
}
//**************************************************************************************//
//**************************************************************************************//
int init_d3d9()
{
   g_D3D = Direct3DCreate9( D3D_SDK_VERSION);
   if(!g_D3D){
      //Handle error'
		return -1;
   }
   return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int init_d3d9device()
{
	DeviceParams.BackBufferHeight = 480;
	DeviceParams.BackBufferWidth = 640;
	DeviceParams.BackBufferFormat = D3DFMT_UNKNOWN;
	DeviceParams.BackBufferCount = 0;

	DeviceParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	DeviceParams.MultiSampleQuality = 0;

	DeviceParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	DeviceParams.hDeviceWindow = hMainWnd;

	DeviceParams.Windowed = TRUE;

	DeviceParams.EnableAutoDepthStencil = FALSE;
	DeviceParams.AutoDepthStencilFormat = D3DFMT_UNKNOWN;

	DeviceParams.Flags = 0;

	DeviceParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	DeviceParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	HRESULT hr2 = g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hMainWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DeviceParams, &g_d3d_device);

	//set vertex format
	g_d3d_device->SetFVF(vertex_fvf);
	//g_d3d_device->SetFVF(vertex_fvf2);

    //g_d3d_device->SetRenderState( D3DRS_AMBIENT, 
    //    D3DCOLOR_XRGB(10,10,10));

	g_d3d_device->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    g_d3d_device->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
	g_d3d_device->SetRenderState( D3DRS_LIGHTING, TRUE);
	hr2 = g_d3d_device->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int d3d9_init_texture()
{   
	D3DXCreateTextureFromFile(g_d3d_device,   //Direct3D Device
							  L"c:/cx2.png",       //File Name
                             &g_texture);    //Texture handle

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int d3d9_release_texture()
{
	g_texture->Release();

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int d3d9_setup_initial_transformations()
{
		
	D3DXVECTOR3 eye_vector,lookat_vector,up_vector;
	D3DXMATRIX view_matrix;

   //View point is 8 units back on the Z-axis
   eye_vector=D3DXVECTOR3( 0.0f, 0.0f, -4.0 );

   //We are looking towards the origin
   lookat_vector=D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

   //The "up" direction is the positive direction on the y-axis
   up_vector=D3DXVECTOR3(0.0f,1.0f,0.0f);

  D3DXMatrixLookAtLH(&view_matrix,
                      &eye_vector,
                      &lookat_vector,
                      &up_vector);


	HRESULT hr = g_d3d_device->SetTransform(D3DTS_VIEW, &view_matrix);

	D3DXMATRIX projection_matrix;
	float aspect;

   aspect=((float)640.0 / (float)480.0);

   D3DXMatrixPerspectiveFovLH(&projection_matrix, //Result Matrix
                              D3DX_PI/4,          //Field of View, in radians.
                              aspect,             //Aspect ratio
                              1.0f,               //Near view plane
                              100.0f );           //Far view plane

   hr = g_d3d_device->SetTransform(D3DTS_PROJECTION, &projection_matrix);

	D3DVIEWPORT9 view_port;

   view_port.X=0;
   view_port.Y=0;
   view_port.Width=640.0;
   view_port.Height=480.0;
   view_port.MinZ=0.0f;
   view_port.MaxZ=1.0f;

   hr = g_d3d_device->SetViewport(&view_port);

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int d3d9_create_vertex_buffer()
{
	int vertex_size = sizeof(vertex);

	HRESULT hr = g_d3d_device->CreateVertexBuffer(vertex_size * 512, D3DUSAGE_WRITEONLY, vertex_fvf, D3DPOOL_MANAGED, &g_vertex_buffer, NULL);


	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int d3d9_set_vertex_buffer()
{
	vertex* vertices = NULL;

	g_vertex_buffer->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

		vertices[2].x = -1;
		vertices[2].y = -1;
		vertices[2].z = 0;
		vertices[2].nx = 0;
		vertices[2].ny = 0;
		vertices[2].nz = -1;
		vertices[2].tu = 0.0;
		vertices[2].tv = 0.0;
		vertices[2].colour = 0xffffffff;

		vertices[1].x = 1;
		vertices[1].y = -1;
		vertices[1].z = 0;
		vertices[1].nx = 0;
		vertices[1].ny = 0;
		vertices[1].nz = -1;
		vertices[1].tu = 1.0;
		vertices[1].tv = 0.0;
		vertices[1].colour = 0xffffffff;

		vertices[0].x = -1;
		vertices[0].y = 1;
		vertices[0].z = 0;
		vertices[0].nx = 0;
		vertices[0].ny = 0;
		vertices[0].nz = -1;
		vertices[0].tu = 0.0;
		vertices[0].tv = 1.0;
		vertices[0].colour = 0xffffffff;

		vertices[3].x = -1;
		vertices[3].y = -1;
		vertices[3].z = 0;
		vertices[3].nx = 0;
		vertices[3].ny = 0;
		vertices[3].nz = 1;
		vertices[3].tu = 0.0;
		vertices[3].tv = 0.0;
		vertices[3].colour = 0xffffffff;

		vertices[4].x = 1;
		vertices[4].y = -1;
		vertices[4].z = 0;
		vertices[4].nx = 0;
		vertices[4].ny = 0;
		vertices[4].nz = 1;
		vertices[4].tu = 1.0;
		vertices[4].tv = 0.0;
		vertices[4].colour = 0xffffffff;

		vertices[5].x = -1;
		vertices[5].y = 1;
		vertices[5].z = 0;
		vertices[5].nx = 0;
		vertices[5].ny = 0;
		vertices[5].nz = 1;
		vertices[5].tu = 0.0;
		vertices[5].tv = 1.0;
		vertices[5].colour = 0xffffffff;

		vertices[8].x = 1;
		vertices[8].y = 1;
		vertices[8].z = 0;
		vertices[8].nx = 0;
		vertices[8].ny = 0;
		vertices[8].nz = -1;
		vertices[8].tu = 1.0;
		vertices[8].tv = 1.0;
		vertices[8].colour = 0xffffffff;

		vertices[7].x = -1;
		vertices[7].y = 1;
		vertices[7].z = 0;
		vertices[7].nx = 0;
		vertices[7].ny = 0;
		vertices[7].nz = -1;
		vertices[7].tu = 0.0;
		vertices[7].tv = 1.0;
		vertices[7].colour = 0xffffffff;

		vertices[6].x = 1;
		vertices[6].y = -1;
		vertices[6].z = 0;
		vertices[6].nx = 0;
		vertices[6].ny = 0;
		vertices[6].nz = -1;
		vertices[6].tu = 1.0;
		vertices[6].tv = 0.0;
		vertices[6].colour = 0xffffffff;

		vertices[9].x = 1;
		vertices[9].y = 1;
		vertices[9].z = 0;
		vertices[9].nx = 0;
		vertices[9].ny = 0;
		vertices[9].nz = 1;
		vertices[9].tu = 1.0;
		vertices[9].tv = 1.0;
		vertices[9].colour = 0xffffffff;

		vertices[10].x = -1;
		vertices[10].y = 1;
		vertices[10].z = 0;
		vertices[10].nx = 0;
		vertices[10].ny = 0;
		vertices[10].nz = 1;
		vertices[10].tu = 0.0;
		vertices[10].tv = 1.0;
		vertices[10].colour = 0xffffffff;

		vertices[11].x = 1;
		vertices[11].y = -1;
		vertices[11].z = 0;
		vertices[11].nx = 0;
		vertices[11].ny = 0;
		vertices[11].nz = 1;
		vertices[11].tu = 1.0;
		vertices[11].tv = 0.0;
		vertices[11].colour = 0xffffffff;

	g_vertex_buffer->Unlock();

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int d3d9_release_vertex_buffer()
{
	g_vertex_buffer->Release();

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int d3d9_set_lights()
{
    // Set light #0 to be a simple, faint grey directional light so 
    // the walls and floor are slightly different shades of grey
    D3DLIGHT9 light;                 // Description of the D3D light
    ZeroMemory( &light, sizeof(light) );
    light.Type = D3DLIGHT_POINT;
    //light.Direction = D3DXVECTOR3( 0.0, 0.0, 1.0 );
	light.Position = D3DXVECTOR3(0.0f, 1.0f, -1.0f);
	light.Range = 10.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 1.0f;
	light.Attenuation2 = 0.0f;
    light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 1.0f;
    g_d3d_device->SetLight( 0, &light );
	g_d3d_device->LightEnable(0, TRUE);

	D3DMATERIAL9 material;

	ZeroMemory( &material, sizeof(D3DMATERIAL9));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_d3d_device->SetMaterial(&material);

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
VOID D3DUtil_InitMaterial( D3DMATERIAL9& mtrl, FLOAT r, FLOAT g, FLOAT b,
                           FLOAT a )
{
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = r;
    mtrl.Diffuse.g = mtrl.Ambient.g = g;
    mtrl.Diffuse.b = mtrl.Ambient.b = b;
    mtrl.Diffuse.a = mtrl.Ambient.a = a;
}
//**************************************************************************************//
//**************************************************************************************//
int d3d9_render()
{

	HRESULT hr = g_d3d_device->Clear(0,  //Number of rectangles to clear, we're clearing everything so set it to 0
                          NULL, //Pointer to the rectangles to clear, NULL to clear whole display
                          D3DCLEAR_TARGET,   //What to clear.  We don't have a Z Buffer or Stencil Buffer
                          0x00000000, //Colour to clear to (AARRGGBB)
                          1.0f,  //Value to clear ZBuffer to, doesn't matter since we don't have one
                          0 );   //Stencil clear value, again, we don't have one, this value doesn't matter
	if (hr != S_OK)
		return -1;

	hr = g_d3d_device->BeginScene();

	if (hr != S_OK)
		return -1;

	g_d3d_device->SetTexture(0,g_texture);
	g_d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
    g_d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    g_d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_CURRENT); 

    g_d3d_device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_NONE); 
    g_d3d_device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_NONE); 

	//g_d3d_device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	g_d3d_device->SetStreamSource(0,                   //StreamNumber
                                 g_vertex_buffer,           //StreamData
                                 0,                   //OffsetInBytes
                                 sizeof(vertex)); //Stride

	d3d9_setup_initial_transformations();

	D3DMATRIX transform1;
	
	dhMatrixRotationY(&transform1, angle);
	g_d3d_device->SetTransform(D3DTS_WORLD, &transform1);

	angle = angle + 0.01;

	g_d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

	hr = g_d3d_device->EndScene();
	if (hr != S_OK)
		return -1;

	hr = g_d3d_device->Present(NULL, NULL, NULL, NULL);
	if (hr != S_OK)
		return -1;


	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int exit_d3d9device()
{
	if (g_d3d_device != NULL)
	{
		g_d3d_device->Release();
		g_d3d_device = NULL;
	}
	
	return 0;
}
//**************************************************************************************//
//**************************************************************************************//
int exit_d3d9()
{
	   //Then at the end of your application
   if(g_D3D){
      g_D3D->Release();
      g_D3D=NULL;
   }

   return 0;

}
//**************************************************************************************//
//**************************************************************************************//

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	bRunFlag = true;

	hAppInstance = hInstance;
	create_window();
	init_d3d9();
	init_d3d9device();
	d3d9_init_texture();
	d3d9_create_vertex_buffer();
	d3d9_set_lights();

	//d3d9_setup_initial_transformations();

	MSG WinMsg;
	while (bRunFlag == true)
	{
			d3d9_set_vertex_buffer();
			d3d9_render();

			int iRet = PeekMessage(&WinMsg, NULL, 0, 0, PM_REMOVE);
			while (iRet != 0)
			{
				TranslateMessage(&WinMsg);
				DispatchMessage(&WinMsg);
				iRet = PeekMessage(&WinMsg, NULL, 0, 0, PM_REMOVE);
			} 
	}

	d3d9_release_texture();
	d3d9_release_vertex_buffer();
	exit_d3d9device();
	exit_d3d9();

	return 0;
}
//**************************************************************************************//
//**************************************************************************************//

LRESULT CALLBACK WndCallBackProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
		{
		case WM_KEYDOWN:
			switch(wParam)
				{
				case VK_ESCAPE:
					bRunFlag = 0;
					break;
				}
			break;
		case WM_CLOSE:
			bRunFlag = 0;
		break;
		}
	  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//**************************************************************************************//
//**************************************************************************************//