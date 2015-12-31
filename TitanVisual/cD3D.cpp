#include "cD3D.h"
//*******************************************************//
//*******************************************************//
int cD3D::init()
{

	D3D =  Direct3DCreate9( D3D_SDK_VERSION);
   if(D3D == NULL)
   {
   		return -1;
   }

	for (int lc1 = 0; lc1 < MAX_D3D_DEVICES; lc1++)
	{
		Devices[lc1].init(lc1);
	}

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D::exit()
{
	for (int lc1 = 0; lc1 < MAX_D3D_DEVICES; lc1++)
	{
		Devices[lc1].exit();
	}
	
	if(D3D != NULL)
	{
      D3D->Release();
      D3D=NULL;
   }

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D::load_window_3d(int iWindow, HWND _hWindow, int _iWidth, int _iHeight)
{
	if (iWindow < 0)
		return -1;
	if (iWindow >= MAX_D3D_DEVICES)
		return -1;

	Devices[iWindow].iHeight = _iHeight;
	Devices[iWindow].iWidth = _iWidth;

	Devices[iWindow].DeviceParams.BackBufferHeight = _iHeight;
	Devices[iWindow].DeviceParams.BackBufferWidth = _iWidth;
	Devices[iWindow].DeviceParams.BackBufferFormat = D3DFMT_UNKNOWN;
	Devices[iWindow].DeviceParams.BackBufferCount = 0;

	Devices[iWindow].DeviceParams.MultiSampleType = D3DMULTISAMPLE_NONE;//D3DMULTISAMPLE_8_SAMPLES; //D3DMULTISAMPLE_NONE;
	Devices[iWindow].DeviceParams.MultiSampleQuality = 0;

	Devices[iWindow].DeviceParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	Devices[iWindow].DeviceParams.hDeviceWindow = _hWindow;

	Devices[iWindow].DeviceParams.Windowed = TRUE;

	Devices[iWindow].DeviceParams.EnableAutoDepthStencil = TRUE;
	Devices[iWindow].DeviceParams.AutoDepthStencilFormat = D3DFMT_D24X8; //D3DFMT_D16;

	Devices[iWindow].DeviceParams.Flags = 0;

	Devices[iWindow].DeviceParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	Devices[iWindow].DeviceParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Note D3DCREATE_FPU_PRESERVE
	//If you don't do this, Direct3D can somehow change the precision of *all* following
	//floating point calculations to single precision. Which will have some painful 
	//consequences when trying to show satellites in orbit...
	HRESULT hr2 = D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE, &Devices[iWindow].DeviceParams, &Devices[iWindow].Device);

	Devices[iWindow].init_renderstate_3d();

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D::load_window_2d(int iWindow, HWND _hWindow, int _iWidth, int _iHeight)
{
	if (iWindow < 0)
		return -1;
	if (iWindow >= MAX_D3D_DEVICES)
		return -1;

	Devices[iWindow].iHeight = _iHeight;
	Devices[iWindow].iWidth = _iWidth;

	Devices[iWindow].DeviceParams.BackBufferHeight = _iHeight;
	Devices[iWindow].DeviceParams.BackBufferWidth = _iWidth;
	Devices[iWindow].DeviceParams.BackBufferFormat = D3DFMT_UNKNOWN;
	Devices[iWindow].DeviceParams.BackBufferCount = 0;

	Devices[iWindow].DeviceParams.MultiSampleType = D3DMULTISAMPLE_NONE;//D3DMULTISAMPLE_8_SAMPLES; //D3DMULTISAMPLE_NONE;
	Devices[iWindow].DeviceParams.MultiSampleQuality = 0;

	Devices[iWindow].DeviceParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	Devices[iWindow].DeviceParams.hDeviceWindow = _hWindow;

	Devices[iWindow].DeviceParams.Windowed = TRUE;

	Devices[iWindow].DeviceParams.EnableAutoDepthStencil = TRUE;
	Devices[iWindow].DeviceParams.AutoDepthStencilFormat = D3DFMT_D16;

	Devices[iWindow].DeviceParams.Flags = 0;

	Devices[iWindow].DeviceParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	Devices[iWindow].DeviceParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Note D3DCREATE_FPU_PRESERVE
	//If you don't do this, Direct3D can somehow change the precision of *all* following
	//floating point calculations to single precision. Which will have some painful 
	//consequences when trying to show satellites in orbit...
	HRESULT hr2 = D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE, &Devices[iWindow].DeviceParams, &Devices[iWindow].Device);

	if (hr2 == D3DERR_DEVICELOST)
	{
		printf ("D3DERR_DEVICELOST\n");
	}
	if (hr2 ==  D3DERR_INVALIDCALL)
	{
		printf ("D3DERR_INVALIDCALL\n");
	}
	if (hr2 == D3DERR_NOTAVAILABLE)
	{
		printf("D3DERR_NOTAVAILABLE\n");
	}


	Devices[iWindow].init_renderstate_2d();

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D::render(VECTOR3 start_offset0, VECTOR3 start_offset1,  double scale1, double sim_time)
{
	if (Devices[0].Device != NULL)
		Devices[0].render0(start_offset0, sim_time);
	if (Devices[1].Device != NULL)
		Devices[1].render1(start_offset1, scale1, sim_time);

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::init(int _iDeviceID)
{
	Device = NULL;
	iDeviceID = _iDeviceID;

	m_font = NULL;

	RenderList = new cRenderObject();
	RenderList->init();

	iHeight = 0;
	iWidth = 0;

	cam_yaw = 0; 
	cam_tilt = 0;
	cam_dist = 20;

	bViewChanged = false;
	bFollow = false;

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::exit()
{
	if (RenderList != NULL)
	{
		RenderList->exit();
		delete RenderList;
		RenderList = NULL;
	}

	if (m_font != NULL)
	{
		m_font->Release();
		m_font = NULL;
	}

	if (Device != NULL)
	{
		Device->Release();
		Device = NULL;
	}		

	return 0;
}
//*******************************************************//
//*******************************************************//
D3DMATRIX cD3D_Device::GetD3DMatrix(MATRIX input)
{
	D3DMATRIX ret;

	ret._11 = input._11;
	ret._12 = input._12;
	ret._13 = input._13;
	ret._14 = input._14;

	ret._21 = input._21;
	ret._22 = input._22;
	ret._23 = input._23;
	ret._24 = input._24;

	ret._31 = input._31;
	ret._32 = input._32;
	ret._33 = input._33;
	ret._34 = input._34;

	ret._41 = input._41;
	ret._42 = input._42;
	ret._43 = input._43;
	ret._44 = input._44;

	return ret;
}
//*******************************************************//
//*******************************************************//
//render a history line with the supplied history buffer
int cD3D_Device::render_history_line(cHistoryBuffer* pHistBuff, VECTOR3 position_offset, MATRIX CurrTransform)
{
	//keep track of the number of vertexes we're actually going to render
	int iNumVertexes = 0;

	//fill the vertex buffer with the real points we're going to use
	for (int i = 1; i < HISTORY_BUFFER_SIZE; i++)
	{
		//calculate the actual index we're using
		int index = pHistBuff->curr_index - i;
		//wrap around
		if (index < 0)
			index += HISTORY_BUFFER_SIZE;

		//break if this point is not enabled
		if (pHistBuff->Points[index].bValid == false)
			break;

		if (i > 1)
		{
				int prev_index = index + 1;
				//wrap around
				if (prev_index >= HISTORY_BUFFER_SIZE)
					prev_index -= HISTORY_BUFFER_SIZE;

				//otherwise put it's data into the buffer
				pHistBuff->Vertexes[iNumVertexes].x = pHistBuff->Points[prev_index].px + position_offset.x;
				pHistBuff->Vertexes[iNumVertexes].y = pHistBuff->Points[prev_index].py + position_offset.y;
				pHistBuff->Vertexes[iNumVertexes].z = pHistBuff->Points[prev_index].pz + position_offset.z;
				iNumVertexes++;

				//otherwise put it's data into the buffer
				pHistBuff->Vertexes[iNumVertexes].x = pHistBuff->Points[index].px + position_offset.x;
				pHistBuff->Vertexes[iNumVertexes].y = pHistBuff->Points[index].py + position_offset.y;
				pHistBuff->Vertexes[iNumVertexes].z = pHistBuff->Points[index].pz + position_offset.z;
				//increment vertex count
				iNumVertexes++;
		}
	}
	
	//if we had less than 2 vertexes, exit early sicne we can't draw it
	if (iNumVertexes < 2)
		return 0;

	//declare a raw pointer to retrieve the vertex buffer from D3D
	vertex* pVert = NULL;

	//now "lock in" the raw history data and get the pointer
	pHistBuff->VertexBuffer->Lock(0, 0, (void**)&pVert, D3DLOCK_DISCARD);

	//and now copy over the data from our "pre" vertex buffer
	memcpy(pVert, pHistBuff->Vertexes, sizeof(vertex)*iNumVertexes);

	//unlock the buffer
	pHistBuff->VertexBuffer->Unlock();

	//now render this buffer
		//set transform
		D3DMATRIX TransformInput = GetD3DMatrix(create_identity_matrix());//CurrTransform);
		HRESULT hr = Device->SetTransform(D3DTS_WORLD, &TransformInput);

		hr = Device->SetMaterial(&pHistBuff->Material);

		//set up stream location
		hr = Device->SetStreamSource(0, pHistBuff->VertexBuffer, 0, sizeof(vertex)); 
		//actually render
		hr = Device->DrawPrimitive(D3DPT_LINELIST, 0, iNumVertexes / 2);

	//done.
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::render_history_line_2d(cHistoryBuffer* pHistBuff, double x_offset, double y_offset, double input_scale)
{
	//keep track of the number of vertexes we're actually going to render
	int iNumVertexes = 0;

	//fill the vertex buffer with the real points we're going to use
	for (int i = 1; i < HISTORY_BUFFER_SIZE; i++)
	{
		//calculate the actual index we're using
		int index = pHistBuff->curr_index - i;
		//wrap around
		if (index < 0)
			index += HISTORY_BUFFER_SIZE;

		//break if this point is not enabled
		if (pHistBuff->Points[index].bValid == false)
			break;

		//do a range check  (to prevent the history line from wrapping around)
		if (i > 1)
		{
			int prev_index = index + 1;
			//wrap around
			if (prev_index >= HISTORY_BUFFER_SIZE)
				prev_index -= HISTORY_BUFFER_SIZE;

			//compute differences
			double dx = pHistBuff->Points[index].px - pHistBuff->Points[prev_index].px;
			double dy = pHistBuff->Points[index].py - pHistBuff->Points[prev_index].py;

			double ds = dx*dx + dy*dy;
			double s = 0;
			if (ds > 0)
				s = sqrt(ds);

			//only add if we don't exceed a certain length
			//this is different from the 3D history line implementation
			//in that we are sending data to the buffer as lines, and not a line strip
			if (s < 5.0)
			{
				//send both the previous point and the current one
				pHistBuff->Vertexes[iNumVertexes].x = (pHistBuff->Points[prev_index].px + x_offset - 320) * input_scale + 320;
				pHistBuff->Vertexes[iNumVertexes].y = (pHistBuff->Points[prev_index].py + y_offset - 240) * input_scale + 240;
				pHistBuff->Vertexes[iNumVertexes].z = 0.5;
				//increment vertex count
				iNumVertexes++;

				pHistBuff->Vertexes[iNumVertexes].x = (pHistBuff->Points[index].px + x_offset - 320) * input_scale + 320;
				pHistBuff->Vertexes[iNumVertexes].y = (pHistBuff->Points[index].py + y_offset - 240) * input_scale + 240;
				pHistBuff->Vertexes[iNumVertexes].z = 0.5;
				//increment vertex count
				iNumVertexes++;

				//wrap around line segments
				if ((pHistBuff->Points[index].px + x_offset) < 0.0)
				{
					pHistBuff->Vertexes[iNumVertexes-1].x += 640.0 * input_scale;
					pHistBuff->Vertexes[iNumVertexes-2].x += 640.0 * input_scale;
				}
				if ((pHistBuff->Points[index].py + y_offset) < 0.0)
				{
					pHistBuff->Vertexes[iNumVertexes-1].y += 480.0 * input_scale;
					pHistBuff->Vertexes[iNumVertexes-2].y += 480.0 * input_scale;
				}
				if ((pHistBuff->Points[index].px + x_offset) > 640.0)
				{
					pHistBuff->Vertexes[iNumVertexes-1].x -= 640.0 * input_scale;
					pHistBuff->Vertexes[iNumVertexes-2].x -= 640.0 * input_scale;
				}
				if ((pHistBuff->Points[index].py + y_offset) > 480.0)
				{
					pHistBuff->Vertexes[iNumVertexes-1].y -= 480.0 * input_scale;
					pHistBuff->Vertexes[iNumVertexes-2].y -= 480.0 * input_scale;
				}
			}
		}
	}
	
	//if we had less than 2 vertexes, exit early sicne we can't draw it
	if (iNumVertexes < 2)
		return 0;

	//declare a raw pointer to retrieve the vertex buffer from D3D
	vertex2d* pVert = NULL;

	//now "lock in" the raw history data and get the pointer
	pHistBuff->VertexBuffer->Lock(0, 0, (void**)&pVert, D3DLOCK_DISCARD);

	//note: we're doing it this way because "vertexes" is actually the 3D fvf buffer,
	//while pVert is the 2D buffer. Since we're just copying over the x and y coordinates, I thought
	//this was fine...for now.
	for (int lc0 = 0; lc0 < iNumVertexes; lc0++)
	{
		pVert[lc0].x = pHistBuff->Vertexes[lc0].x;
		pVert[lc0].y = pHistBuff->Vertexes[lc0].y;
		pVert[lc0].z = pHistBuff->Vertexes[lc0].z;
		pVert[lc0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVert[lc0].rhw = 1.0f;
	}
	//unlock the buffer
	pHistBuff->VertexBuffer->Unlock();

	//now render this buffer
		HRESULT hr = Device->SetMaterial(&pHistBuff->Material);

		//set up stream location
		hr = Device->SetStreamSource(0, pHistBuff->VertexBuffer, 0, sizeof(vertex2d)); 
		//actually render  (not it's LINELIST isntead of LINESTRIP)
		hr = Device->DrawPrimitive(D3DPT_LINELIST, 0, iNumVertexes / 2);


	//done.
	return 0;
}
//*******************************************************//
//*******************************************************//
//adds a history buffer to a render object 
int cD3D_Device::add_history_buffer(cRenderObject* input_object, bool initial_enable_status, D3DXCOLOR history_line_color)
{
	//allocate a new history buffer
	input_object->pHistoryBuffer = new cHistoryBuffer();
	//set it's initial enable status
	input_object->pHistoryBuffer->bEnable = initial_enable_status;
	
	//initialize it's vertex buffer
	init_vertex_buffer_for_history(input_object->pHistoryBuffer, HISTORY_BUFFER_SIZE);
	
	//initialize it's material
	ZeroMemory( &input_object->pHistoryBuffer->Material, sizeof(D3DMATERIAL9));

	input_object->pHistoryBuffer->Material.Diffuse = history_line_color;
	input_object->pHistoryBuffer->Material.Ambient = history_line_color;
	input_object->pHistoryBuffer->Material.Emissive = history_line_color;


	//done.
	return 0;
}
//*******************************************************//
//*******************************************************//
//adds a history buffer to a render object, 2D version
int cD3D_Device::add_history_buffer_2d(cRenderObject* input_object, bool initial_enable_status, D3DXCOLOR history_line_color)
{
	//allocate a new history buffer
	input_object->pHistoryBuffer = new cHistoryBuffer();
	//set it's initial enable status
	input_object->pHistoryBuffer->bEnable = initial_enable_status;
	
	//initialize it's vertex buffer
	init_vertex_buffer_for_history_2d(input_object->pHistoryBuffer, HISTORY_BUFFER_SIZE);
	
	//initialize it's material
	ZeroMemory( &input_object->pHistoryBuffer->Material, sizeof(D3DMATERIAL9));

	input_object->pHistoryBuffer->Material.Diffuse = history_line_color;
	input_object->pHistoryBuffer->Material.Ambient = history_line_color;
	input_object->pHistoryBuffer->Material.Emissive = history_line_color;

	//done.
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::render_object(cRenderObject* object_to_render, MATRIX BaseTransform)
{
	HRESULT hr = 0;

	if (object_to_render == NULL)
		return -1;

	//DEBUG
	//fprintf(dfile, "%s, %f, %f, %f,", object_to_render->Name, BaseTransform._41, BaseTransform._42,BaseTransform._43);

	//calculate transform
	if ((object_to_render->iOrientationMode & ORIENTATION_MODE_BODYXYZ) != 0)
		BaseTransform = apply_body_translation(BaseTransform, object_to_render->px, object_to_render->py, object_to_render->pz);
	if ((object_to_render->iOrientationMode & ORIENTATION_MODE_LATLONGPR) != 0)
		BaseTransform = apply_body_latlongpr(BaseTransform, object_to_render->latitude, object_to_render->longitude, object_to_render->pr);
	if ((object_to_render->iOrientationMode & ORIENTATION_MODE_PITCHROLLYAW) != 0)
		BaseTransform = apply_body_pitchrollyaw(BaseTransform, object_to_render->pitch, object_to_render->roll, object_to_render->yaw);
	
	//save the transformed position values
	//note that we apply the original start offsets to retrieve the right "initial" position.
	object_to_render->abs_position.x = BaseTransform._41 - save_start_offset.x;
	object_to_render->abs_position.y = BaseTransform._42 - save_start_offset.y;
	object_to_render->abs_position.z = BaseTransform._43 - save_start_offset.z;

	if (object_to_render->VertexBuffer != NULL)
	{
		//set transform
		D3DMATRIX TransformInput = GetD3DMatrix(BaseTransform);
		hr = Device->SetTransform(D3DTS_WORLD, &TransformInput);

		//render self
		hr = Device->SetStreamSource(0, object_to_render->VertexBuffer, 0, sizeof(vertex)); 
		
		//if (object_to_render->Texture != NULL)
		hr = Device->SetTexture(0,object_to_render->Texture);

		hr = Device->SetMaterial(&object_to_render->Material);

		hr = Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, object_to_render->iNumVertexes / 3);
	}

	//if the render object has a history buffer, and it's enabled, render it here
	if (object_to_render->pHistoryBuffer != NULL)
	if (object_to_render->pHistoryBuffer->bEnable == true)
	{
		//get the last point in history
		VECTOR3 last_point = object_to_render->pHistoryBuffer->get_last_point();

		//only add a new point to the history if it changed
		//if ((render_count_0 % 5) == 0)
		if ((last_point.x != object_to_render->abs_position.x) || 
		   (last_point.y != object_to_render->abs_position.y) ||
			(last_point.z != object_to_render->abs_position.z))
				object_to_render->pHistoryBuffer->add_point(object_to_render->abs_position.x, object_to_render->abs_position.y, object_to_render->abs_position.z);

		//render history line
		render_history_line(object_to_render->pHistoryBuffer, save_start_offset, BaseTransform);
	}

	cRenderObject* pTrav = object_to_render->pFirstChild;

	while (pTrav != NULL)
	{
		render_object(pTrav, BaseTransform);
		pTrav = pTrav->pNextSibling;
	}

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::update_vertex_buffer_2d_alt(cRenderObject* input_object, double x_offset, double y_offset, double input_scale)
{

	//declare a pointer to receive the "real" vertex buffer location
	//once locked
	vertex2d* vertices = NULL;

	double min_x = 0;
	double min_y = 0;
	double max_x = 0;
	double max_y = 0;
	double o_max_tu = 0;
	double o_max_tv = 0;

	//determine min and max, x and y
	for (int lc1 = 0; lc1 < input_object->iNumVertexes; lc1++)
	{
		double x = (input_object->Vertexes2d[lc1].x + x_offset - 320) * input_scale + 320;
		double y = (input_object->Vertexes2d[lc1].y + y_offset - 240) * input_scale + 240;
		double tu = input_object->Vertexes2d[lc1].tu1;
		double tv = input_object->Vertexes2d[lc1].tv1;

		if (x > max_x)
			max_x = x;
		if (x < min_x)
			min_x = x;
		if (y > max_y)
			max_y = y;
		if (y < min_y)
			min_y = y;
		if (tu > o_max_tu)
			o_max_tu = tu;
		if (tv > o_max_tv)
			o_max_tv = tv;
	}

	//determine tu and tv to use
	double min_tu = (-320.0 - min_x) / (max_x - min_x);
	double min_tv = (-240.0 - min_y) / (max_y - min_y);
	double max_tu = (320.0 - min_x) / (max_x - min_x);
	double max_tv = (240.0 - min_y) / (max_y - min_y);

	//now lock the pointer
	input_object->VertexBuffer->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	vertices[0].x = 320.0;
	vertices[0].y = 240.0;
	vertices[0].z = input_object->Vertexes2d[0].z;
	vertices[0].rhw  = input_object->Vertexes2d[0].rhw;
	vertices[0].Diffuse = input_object->Vertexes2d[0].Diffuse;
	vertices[0].tu1 = max_tu;
	vertices[0].tv1 = min_tv;

	vertices[1].x = 320.0;
	vertices[1].y = -240.0;
	vertices[1].z = input_object->Vertexes2d[1].z;
	vertices[1].rhw  = input_object->Vertexes2d[1].rhw;
	vertices[1].Diffuse = input_object->Vertexes2d[1].Diffuse;
	vertices[1].tu1 = max_tu;
	vertices[1].tv1 = max_tv;

	vertices[2].x = -320.0;
	vertices[2].y = -240.0;
	vertices[2].z = input_object->Vertexes2d[2].z;
	vertices[2].rhw  = input_object->Vertexes2d[2].rhw;
	vertices[2].Diffuse = input_object->Vertexes2d[2].Diffuse;
	vertices[2].tu1 = min_tu;
	vertices[2].tv1 = max_tv;

	vertices[3].x = -320.0;
	vertices[3].y = 240.0;
	vertices[3].z = input_object->Vertexes2d[3].z;
	vertices[3].rhw  = input_object->Vertexes2d[3].rhw;
	vertices[3].Diffuse = input_object->Vertexes2d[3].Diffuse;
	vertices[3].tu1 = min_tu;
	vertices[3].tv1 = min_tv;

	//unlock 
	input_object->VertexBuffer->Unlock();

	//done.
	return 0;

}
//*******************************************************//
//*******************************************************//
//re-copy the contents of the local vertex storage to the
//actual vertex buffer, adding the specified x_offset and y_offset
int cD3D_Device::update_vertex_buffer_2d(cRenderObject* input_object, double x_offset, double y_offset, double input_scale)
{
	//declare a pointer to receive the "real" vertex buffer location
	//once locked
	vertex2d* vertices = NULL;

	//now lock the pointer
	input_object->VertexBuffer->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);
	

	double x_wrap_offset = 0;
	double y_wrap_offset = 0;
	
	if (input_object->bFixedScale2D == true)
	{
		if ((input_object->Vertexes2d[0].x + x_offset) < 0.0)
			x_wrap_offset = 640.0;
		if ((input_object->Vertexes2d[0].x + x_offset) > 640.0)
			x_wrap_offset = -640.0;

		if ((input_object->Vertexes2d[0].y + y_offset) < 0.0)
			y_wrap_offset = 480.0;
		if ((input_object->Vertexes2d[0].y + y_offset) > 480.0)
			y_wrap_offset = -480.0;

	}

	//copy over the vertices
	for (int lc1 = 0; lc1 < input_object->iNumVertexes; lc1++)
	{
		//copy over each one
		if (input_object->bFixedScale2D == false)
		{
			vertices[lc1].x = (input_object->Vertexes2d[lc1].x + x_offset - 320) * input_scale + 320;
			vertices[lc1].y = (input_object->Vertexes2d[lc1].y + y_offset - 240) * input_scale + 240;
		} else
		{
			vertices[lc1].x = (x_offset - 320 + x_wrap_offset) * input_scale + 320 + input_object->Vertexes2d[lc1].x;
			vertices[lc1].y = (y_offset - 240 + y_wrap_offset) * input_scale + 240 + input_object->Vertexes2d[lc1].y;
		}
		vertices[lc1].z = input_object->Vertexes2d[lc1].z;
		vertices[lc1].rhw  = input_object->Vertexes2d[lc1].rhw;
		vertices[lc1].Diffuse = input_object->Vertexes2d[lc1].Diffuse;
		vertices[lc1].tu1 = input_object->Vertexes2d[lc1].tu1;
		vertices[lc1].tv1 = input_object->Vertexes2d[lc1].tv1;

	}

	//unlock 
	input_object->VertexBuffer->Unlock();
 
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::render_object_2d(cRenderObject* object_to_render, MATRIX BaseTransform, double input_scale)
{
	HRESULT hr = 0;

	if (object_to_render == NULL)
		return -1;
	
	double old_x_offset = BaseTransform._41;
	double old_y_offset = BaseTransform._42;

	if (object_to_render->VertexBuffer != NULL)
	{
		//add the object's own x and y offsets to the transform
		BaseTransform._41 += object_to_render->px;
		BaseTransform._42 += object_to_render->py;
		BaseTransform._43 += object_to_render->pz;

		//update the vertex buffer
		update_vertex_buffer_2d(object_to_render, BaseTransform._41, BaseTransform._42, input_scale);

		//disble the "alt" option for now...doesn't seem to work
		//if ((object_to_render->bFixedScale2D == true) && (input_scale > 1.0))
		//	update_vertex_buffer_2d_alt(object_to_render, BaseTransform._41, BaseTransform._42, input_scale);
		//else

		//render self
		hr = Device->SetStreamSource(0, object_to_render->VertexBuffer, 0, sizeof(vertex2d)); 
		
		if (object_to_render->Texture != NULL)
			hr = Device->SetTexture(0,object_to_render->Texture);
		//hr = Device->SetMaterial(&object_to_render->Material);

		//if 3 polys its a triangle
		if (object_to_render->iNumVertexes == 3)
			hr = Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		//if 4 its a square (triangle strip, that is 2 triangles with 4 points)
		if (object_to_render->iNumVertexes == 4)
			hr = Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}

	//if the render object has a history buffer, and it's enabled, render it here
	if (object_to_render->pHistoryBuffer != NULL)
	if (object_to_render->pHistoryBuffer->bEnable == true)
	{
		//get the last point in history
		VECTOR3 last_point = object_to_render->pHistoryBuffer->get_last_point();

		//only add a new point to the history if it changed
		//if ((render_count_1 % 5) == 0)
		if ((last_point.x != object_to_render->px) || 
		   (last_point.y != object_to_render->py))
				object_to_render->pHistoryBuffer->add_point(object_to_render->px, object_to_render->py, object_to_render->pz);

		//render history line
		render_history_line_2d(object_to_render->pHistoryBuffer, old_x_offset, old_y_offset, input_scale);
	}

	cRenderObject* pTrav = object_to_render->pFirstChild;

	while (pTrav != NULL)
	{
		render_object_2d(pTrav, BaseTransform, input_scale);
		pTrav = pTrav->pNextSibling;
	}

	return 0;
}
//*******************************************************//
//*******************************************************//
//this is the 2d version of the vertex buffer initializer
int cD3D_Device::init_vertex_buffer_2d(cRenderObject* pObject, int _iNumVertexes)
{	
	int vertex_size = sizeof(vertex2d);
	pObject->iNumVertexes = _iNumVertexes;

	HRESULT hr = Device->CreateVertexBuffer(vertex_size * pObject->iNumVertexes, D3DUSAGE_WRITEONLY, vertex_2d, D3DPOOL_MANAGED, &pObject->VertexBuffer, NULL);

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::init_vertex_buffer(cRenderObject* pObject, int _iNumVertexes)
{	
	int vertex_size = sizeof(vertex);
	pObject->iNumVertexes = _iNumVertexes;

	HRESULT hr = Device->CreateVertexBuffer(vertex_size * pObject->iNumVertexes, D3DUSAGE_WRITEONLY, vertex_fvf, D3DPOOL_MANAGED, &pObject->VertexBuffer, NULL);

	return 0;
}
//*******************************************************//
//*******************************************************//
//this version adds a buffer for use by the history line
int cD3D_Device::init_vertex_buffer_for_history(cHistoryBuffer* pHistBuff, int _iNumVertexes)
{
	int vertex_size = sizeof(vertex);

	HRESULT hr = Device->CreateVertexBuffer(vertex_size * HISTORY_BUFFER_SIZE, D3DUSAGE_WRITEONLY, vertex_fvf, D3DPOOL_MANAGED, &pHistBuff->VertexBuffer, NULL);

	//done.
	return 0;
}
//*******************************************************//
//*******************************************************//
//this version adds a buffer for use by the history line, 2d version
int cD3D_Device::init_vertex_buffer_for_history_2d(cHistoryBuffer* pHistBuff, int _iNumVertexes)
{
	int vertex_size = sizeof(vertex2d);

	HRESULT hr = Device->CreateVertexBuffer(vertex_size * HISTORY_BUFFER_SIZE, D3DUSAGE_WRITEONLY, vertex_2d, D3DPOOL_MANAGED, &pHistBuff->VertexBuffer, NULL);

	//done.
	return 0;
}
//*******************************************************//
//*******************************************************//
//obsolete.
D3DXVECTOR3 cD3D_Device::calc_lookat()
{
	D3DXVECTOR3 base = eye_vector;

	//rotate yaw
	base.x += sin(cam_yaw)*cos(cam_tilt);
	base.y += cos(cam_yaw)*cos(cam_tilt);
	base.z += sin(cam_tilt);
	
	return base;
}
//*******************************************************//
//*******************************************************//
//obsolete.
D3DXVECTOR3 cD3D_Device::calc_eye()
{
	D3DXVECTOR3 base = D3DXVECTOR3(0,0,0);

	//rotate yaw
	base.x = cam_dist*cos(cam_tilt)*sin(cam_yaw);
	base.y = cam_dist*cos(cam_tilt)*cos(cam_yaw);
	base.z = cam_dist*sin(cam_tilt);

	return base;
}

//*******************************************************//
//*******************************************************//
int cD3D_Device::set_z_planes(double zNear, double zFar)
{
	D3DXMATRIX projection_matrix;
	float aspect;

   aspect=((float)iWidth / (float)iHeight);

   D3DXMatrixPerspectiveFovLH(&projection_matrix, //Result Matrix
                              D3DX_PI/4,          //Field of View, in radians.
                              aspect,             //Aspect ratio
                              zNear,               //Near view plane
                              zFar );           //Far view plane

   HRESULT hr = Device->SetTransform(D3DTS_PROJECTION, &projection_matrix);

	//done.
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::init_views0()
{
	D3DXMATRIX view_matrix;

   //View point is 8 units back on the y-axis
   eye_vector=D3DXVECTOR3( 0.0f, -24.0f, 0 );

   //We are looking towards the origin
   lookat_vector=calc_lookat();

   //The "up" direction is the positive direction on the z-axis
   up_vector=D3DXVECTOR3(0.0f,0.0f,1.0f);

  D3DXMatrixLookAtRH(&view_matrix,
                      &eye_vector,
                      &lookat_vector,
                      &up_vector);

	HRESULT hr = Device->SetTransform(D3DTS_VIEW, &view_matrix);

	D3DXMATRIX projection_matrix;
	float aspect;

   aspect=((float)iWidth / (float)iHeight);

   D3DXMatrixPerspectiveFovLH(&projection_matrix, //Result Matrix
                              D3DX_PI/4,          //Field of View, in radians.
                              aspect,             //Aspect ratio
                              1.0f,               //Near view plane
                              4000000.0f );           //Far view plane

   hr = Device->SetTransform(D3DTS_PROJECTION, &projection_matrix);

	D3DVIEWPORT9 view_port;

   view_port.X=0;
   view_port.Y=0;
   view_port.Width=float(iWidth);
   view_port.Height=float(iHeight);
   view_port.MinZ=0.0f;
   view_port.MaxZ=1.0f;

   hr = Device->SetViewport(&view_port);

	return 0;
}
//*******************************************************//
//*******************************************************//
MATRIX cD3D_Device::apply_body_translation(MATRIX input, double px, double py, double pz)
{
	//the original basis vectors
	VECTOR3 xbody0 = VECTOR3(input._11, input._12, input._13);
	VECTOR3 ybody0 = VECTOR3(input._21, input._22, input._23);
	VECTOR3 zbody0 = VECTOR3(input._31, input._32, input._33);

	VECTOR3 translate_vector = VECTOR3(0,0,0);

	translate_vector = xbody0*px + ybody0*py + zbody0*pz;

	input._41 += translate_vector.x;
	input._42 += translate_vector.y;
	input._43 += translate_vector.z;

	return input;
}
//*******************************************************//
//*******************************************************//
int cRenderObject::set_name(char* _Name)
{
	int len = strlen(_Name);
	if (len > 79)
		len = 79;

	memset(Name, 0, 80);
	memcpy(Name, _Name, len);

	return 0;
}
//*******************************************************//
//*******************************************************//
MATRIX cD3D_Device::apply_body_latlongpr(MATRIX input, double latitude, double longitude, double pr)
{
	//note: I am thinking that the matrix subscripts are being interpreted (column, row)
	//rather than (row, column) as I learned in math. 

	//the original basis vectors
	VECTOR3 xbody0 = VECTOR3(input._11, input._12, input._13);
	VECTOR3 ybody0 = VECTOR3(input._21, input._22, input._23);
	VECTOR3 zbody0 = VECTOR3(input._31, input._32, input._33);

	//because of LH coordinate system,
	//longitude applies in the following way:
	//longtiude = 0 -> x = -1, y = 0
	//longitude = +90 (90E) -> x = 0, y = 1
	//longitude = +180 (dateline) -> x = 1, y = 0
	//longitude = +270 (90W) -> x = 0, y = -1

	//assumes body is aligned with forward along x-axis.
	//perform rotation to account for longitude
	VECTOR3 xbody1 = (cos(-longitude) * xbody0) - (sin(-longitude) * ybody0);
	VECTOR3 zbody1 = zbody0;
	VECTOR3 ybody1 = (cos(-longitude) * ybody0) + (sin(-longitude) * xbody0);

	//perform rotation to account for to account for latitude
	VECTOR3 xbody2 = (zbody1 * cos(-latitude)) - (xbody1 * sin(-latitude));
	VECTOR3 zbody2 = (xbody1 * cos(-latitude)) + (zbody1 * sin(-latitude));
	VECTOR3 ybody2 = ybody1;

	//set orientations
	input._11 = xbody2.x;
	input._12 = xbody2.y;
	input._13 = xbody2.z;

	input._21 = ybody2.x;
	input._22 = ybody2.y;
	input._23 = ybody2.z;

	input._31 = zbody2.x;
	input._32 = zbody2.y;
	input._33 = zbody2.z;

	//add translations along body z axis to account for radius
	input._41 += zbody2.x * pr;
	input._42 += zbody2.y * pr;
	input._43 += zbody2.z * pr;  

	return input;
}
//*******************************************************//
//*******************************************************//
MATRIX cD3D_Device::apply_body_pitchrollyaw(MATRIX input, double pitch, double roll, double yaw)
{

	//note: I am thinking that the matrix subscripts are being interpreted (column, row)
	//rather than (row, column) as I learned in math. 

	//the original basis vectors
	VECTOR3 xbody0 = VECTOR3(input._11, input._12, input._13);
	VECTOR3 ybody0 = VECTOR3(input._21, input._22, input._23);
	VECTOR3 zbody0 = VECTOR3(input._31, input._32, input._33);

	//assumes body is aligned with forward along x-axis.
	//perform rotation to account for yaw
	VECTOR3 xbody1 = (xbody0 * cos(yaw)) + (ybody0 * sin(yaw));
	VECTOR3 zbody1 = zbody0;
	VECTOR3 ybody1 = (ybody0 * cos(yaw)) - (xbody0 * sin(yaw));

	//perform rotation to account for to account for pitch
	VECTOR3 xbody2 = (xbody1 * cos(pitch)) - (zbody1 * sin(pitch));
	VECTOR3 zbody2 = (zbody1 * cos(pitch)) + (xbody1 * sin(pitch));
	VECTOR3 ybody2 = ybody1;

	//perform rotation to account for to account for roll
	VECTOR3 ybody3 = (ybody2 * cos(roll)) - (zbody2 * sin(roll));
	VECTOR3 zbody3 = (zbody2 * cos(roll)) + (ybody2 * sin(roll));
	VECTOR3 xbody3 = xbody2;

	//set orientations
	input._11 = xbody3.x;
	input._12 = xbody3.y;
	input._13 = xbody3.z;

	input._21 = ybody3.x;
	input._22 = ybody3.y;
	input._23 = ybody3.z;

	input._31 = zbody3.x;
	input._32 = zbody3.y;
	input._33 = zbody3.z;

	return input;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::reverse_normals(cRenderObject* pObj)
{
	vertex* vertices = NULL;

	pObj->VertexBuffer->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	for (int lc1 = 0; lc1 < pObj->iNumVertexes; lc1++)
	{
		vertices[lc1].nx *= -1;
		vertices[lc1].ny *= -1;
		vertices[lc1].nz *= -1;
	}
	
	pObj->VertexBuffer->Unlock();

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::init_views1()
{
	D3DXVECTOR3 eye_vector,lookat_vector,up_vector;
	D3DXMATRIX view_matrix;

   //View point is 8 units back on the Z-axis
   eye_vector=D3DXVECTOR3( 0.0f, -24.0f, 0 );

   //We are looking towards the origin
   lookat_vector=calc_lookat();

   //The "up" direction is the positive direction on the y-axis
   up_vector=D3DXVECTOR3(0.0f,1.0f,0.0f);

  D3DXMatrixLookAtLH(&view_matrix,
                      &eye_vector,
                      &lookat_vector,
                      &up_vector);


	HRESULT hr = Device->SetTransform(D3DTS_VIEW, &view_matrix);

	D3DXMATRIX projection_matrix;
	float aspect;

   aspect=((float)iWidth / (float)iHeight);

   D3DXMatrixPerspectiveFovLH(&projection_matrix, //Result Matrix
                              D3DX_PI/4,          //Field of View, in radians.
                              aspect,             //Aspect ratio
                              1.0f,               //Near view plane
                              100.0f );           //Far view plane

   hr = Device->SetTransform(D3DTS_PROJECTION, &projection_matrix);

	D3DVIEWPORT9 view_port;

   view_port.X=0;
   view_port.Y=0;
   view_port.Width=iWidth;
   view_port.Height=iHeight;
   view_port.MinZ=0.0f;
   view_port.MaxZ=1.0f;

   hr = Device->SetViewport(&view_port);

	return 0;
}
//*******************************************************//
//*******************************************************//
//load the supplied render object with 2D sprite
int cD3D_Device::load_with_sprite(cRenderObject* input_object, vertex2d* buffer, int _iNumVertexes, WCHAR* TextureFile)
{
	//fail on NULL object
	if (input_object == NULL)
		return -1;

	//reject if not 3 or 4 vertexes
	if (_iNumVertexes > 4)
		return -1;
	if (_iNumVertexes < 3)
		return -1;

	//copy over the passed buffer into local storage
	for (int lc1 =0 ; lc1 < _iNumVertexes; lc1++)
	{
		input_object->Vertexes2d[lc1] = buffer[lc1];
	}

	//initialize the vertex buffer 
	init_vertex_buffer_2d(input_object, _iNumVertexes);	
	
	//save the number of vertexes
	input_object->iNumVertexes = _iNumVertexes;

	if (TextureFile != NULL)
		D3DXCreateTextureFromFile(Device,   //Direct3D Device
							  TextureFile,       //File Name
                              &input_object->Texture);    //Texture handle

	//done.
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::load_with_model(cRenderObject* input_object, vertex* buffer, int _iNumVertexes, WCHAR* TextureFile)
{
	if (input_object == NULL)
		return -1;

	ZeroMemory( &input_object->Material, sizeof(D3DMATERIAL9));
	input_object->Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	input_object->Material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// D3DXMatrixIdentity((D3DXMATRIX*)&input_object->Transform);

	init_vertex_buffer(input_object, _iNumVertexes);

	vertex* vertices = NULL;

	input_object->VertexBuffer->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	for (int lc1 = 0; lc1 < _iNumVertexes; lc1++)
	{
		vertices[lc1] = buffer[lc1];
	}

	input_object->VertexBuffer->Unlock();
	input_object->iNumVertexes = _iNumVertexes;

	if (TextureFile != NULL)
		D3DXCreateTextureFromFile(Device,   //Direct3D Device
							  TextureFile,       //File Name
                         &input_object->Texture);    //Texture handle
	

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::set_views()
{
/*
	if (bViewChanged == false)
		return -1;

	if (bFollow == false)
		lookat_vector=calc_lookat();
	else
		eye_vector=calc_eye();
*/
	D3DXMATRIX view_matrix;

    D3DXMatrixLookAtLH(&view_matrix,
                      &eye_vector,
                      &lookat_vector,
                      &up_vector);


	HRESULT hr = Device->SetTransform(D3DTS_VIEW, &view_matrix);

	if (cam_dist < 5.0)
		set_z_planes(cam_dist*0.2, cam_dist*1000000.0f);
	else if (cam_dist < 20000.0)
		set_z_planes(cam_dist*0.5, 5000000.0f);
	else
		set_z_planes(10000.0f, 5000000.0f);

	bViewChanged = false;

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::eye_tilt_down()
{
	if (cam_tilt > -1.5)
		cam_tilt -= 0.01;

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::eye_forward()
{
	D3DXVECTOR3 diff = lookat_vector - eye_vector;

	diff = diff * 0.3;

	eye_vector = eye_vector + diff;

	bViewChanged = true;
	//bLight0Changed = true;

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::eye_back()
{
	D3DXVECTOR3 diff = lookat_vector - eye_vector;

	diff = diff * 0.3;

	eye_vector = eye_vector - diff;

	bViewChanged = true;
	//bLight0Changed = true;

	return 0;
}	
//*******************************************************//
//*******************************************************//
int cD3D_Device::eye_tilt_up()
{
	if (cam_tilt < 1.5)
		cam_tilt += 0.02;

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::eye_tilt_left()
{
	cam_yaw -= 0.02;
	
	if (cam_yaw < 0)
		cam_yaw = 6.28;

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::eye_tilt_right()
{

	cam_yaw += 0.01;

	if (cam_yaw > 6.28)
		cam_yaw = 0;

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::pan_left()
{

	eye_vector.x -= cos(cam_yaw);
	eye_vector.z += sin(cam_yaw);

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::pan_right()
{

	eye_vector.x += cos(cam_yaw);
	eye_vector.z -= sin(cam_yaw);

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::pan_up()
{

	eye_vector.x -= sin(cam_tilt)*cos(cam_yaw);
	eye_vector.y += cos(cam_tilt);
	eye_vector.z -= sin(cam_tilt)*sin(cam_yaw);

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::pan_down()
{

	eye_vector.x += sin(cam_tilt)*cos(cam_yaw);
	eye_vector.y -= cos(cam_tilt);
	eye_vector.z += sin(cam_tilt)*sin(cam_yaw);

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::follow_on()
{
	eye_vector.x = cam_dist*cos(cam_tilt)*sin(cam_yaw);
	eye_vector.y = cam_dist*sin(cam_tilt);
	eye_vector.z = cam_dist*cos(cam_tilt)*cos(cam_yaw);

	lookat_vector.x = 0;
	lookat_vector.y = 0;
	lookat_vector.z = 0;

	bFollow = true;
	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::zoom_out()
{
	if (cam_dist < 4000000.0)
		cam_dist = cam_dist * 1.1;

	bViewChanged = true;
	//bLight0Changed = true;
	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::zoom_in()
{
	if (cam_dist > 2.0)
		cam_dist = cam_dist * 0.9;
	bViewChanged = true;
	//bLight0Changed = true;


	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::follow_off()
{
	bFollow = false;
	bViewChanged = true;
	//bLight0Changed = true;
	return 0;

}
//*******************************************************//
//*******************************************************//
int cD3D_Device::render_text(double sim_time)
{
		//convert the simulation time to seconds
		int i_seconds = (int)sim_time;

		//get number of hours
		int i_hours = i_seconds / 3600;
		//remove from total
		i_seconds -= i_hours * 3600;
		//get number of minutes
		int i_minutes = i_seconds / 60;
		//remove from total
		i_seconds -= i_minutes*60;

		// Create a colour for the text - in this case blue
		D3DCOLOR fontColor = D3DCOLOR_ARGB(255,255,255,255);    

		// Create a rectangle to indicate where on the screen it should be drawn
		RECT rct;
		rct.left=2;
		rct.right=400;
		rct.top=10;
		rct.bottom=rct.top+180;
		 
		char NText[50];
		NText[0] = 0;
		char OutText[800];

		memset(OutText, 0, 80);
		strcat(OutText, "Time: ");

		NText[0] = 0;
		itoa(i_hours, NText, 10);
		strcat(OutText, NText);
		strcat(OutText, ":");
		itoa(i_minutes, NText, 10);
		if (i_minutes < 10)
			strcat(OutText, "0");
		strcat(OutText, NText);
		strcat(OutText, ":");
		if (i_seconds < 10)
			strcat(OutText, "0");
		itoa(i_seconds, NText, 10);
		strcat(OutText, NText);

		// Draw some text 
		m_font_large->DrawTextA(NULL, OutText, -1, &rct, 0, fontColor );


	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::render_text_2d()
{

		// Create a colour for the text - in this case green
		D3DCOLOR fontColor = D3DCOLOR_ARGB(255,0,255,0);    

		// Create a rectangle to indicate where on the screen it should be drawn
		RECT rct;
		rct.left=2;
		rct.right=200;
		rct.top=10;
		rct.bottom=rct.top+180;
		 
		char NText[50];
		NText[0] = 0;
		char OutText[800];

		memset(OutText, 0, 80);

		strcat(OutText, "Ground Track");

		// Draw some text 
		m_font->DrawTextA(NULL, OutText, -1, &rct, 0, fontColor );


	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::render0(VECTOR3 start_offset, double sim_time)
{

	//increment render count
	render_count_0++;

	//dfile = fopen("c:/output2.csv", "a+");

	//fprintf(dfile, "time, %f,", sim_time);

	set_views();
	set_lights();

	HRESULT hr = Device->Clear(0,  //Number of rectangles to clear, we're clearing everything so set it to 0
                          NULL, //Pointer to the rectangles to clear, NULL to clear whole display
                          D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,   //What to clear.  We don't have a Z Buffer or Stencil Buffer
                          0x00000000, //Colour to clear to (AARRGGBB)
                          1.0f,  //Value to clear ZBuffer to, doesn't matter since we don't have one
                          0 );   //Stencil clear value, again, we don't have one, this value doesn't matter
	if (hr != S_OK)
		return -1;

	hr = Device->BeginScene();

	if (hr != S_OK)
		return -1;

	render_text(sim_time);

	//D3DXMATRIX mat1;
	//D3DXMatrixIdentity(&mat1);
	MATRIX StartMatrix = create_identity_matrix();
	//offset by start offset
	StartMatrix._41 = start_offset.x;
	StartMatrix._42 = start_offset.y;
	StartMatrix._43 = start_offset.z;

	//save start offsets
	save_start_offset = start_offset;

	//now start rendering
	render_object(RenderList, StartMatrix);

	hr = Device->EndScene();
	if (hr != S_OK)
		return -1;

	hr = Device->Present(NULL, NULL, NULL, NULL);

	//fprintf(dfile, "\n");
	//fclose(dfile);

	if (hr != S_OK)
		return -1;




	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::render1(VECTOR3 start_offset, double input_scale, double sim_time)
{
	//dfile = fopen("c:/output2.csv", "a+");

	//fprintf(dfile, "time, %f,", sim_time);

	//set_views();
	//set_lights();
	render_count_1++;

	HRESULT hr = Device->Clear(0,  //Number of rectangles to clear, we're clearing everything so set it to 0
                          NULL, //Pointer to the rectangles to clear, NULL to clear whole display
                          D3DCLEAR_TARGET,   //What to clear.  We don't have a Z Buffer or Stencil Buffer
                          D3DCOLOR_XRGB(0,0,0), //Colour to clear to (AARRGGBB)
                          1.0f,  //Value to clear ZBuffer to, doesn't matter since we don't have one
                          0 );   //Stencil clear value, again, we don't have one, this value doesn't matter
	if (hr != S_OK)
		return -1;

	hr = Device->BeginScene();

	if (hr != S_OK)
		return -1;

		MATRIX StartMatrix = create_identity_matrix();
		//offset by start offset
		StartMatrix._41 = start_offset.x;
		StartMatrix._42 = start_offset.y;
		StartMatrix._43 = start_offset.z;

		//now start rendering
		render_object_2d(RenderList, StartMatrix, input_scale);

		render_text_2d();

		hr = Device->EndScene();
		if (hr != S_OK)
			return -1;

	hr = Device->Present(NULL, NULL, NULL, NULL);

	if (hr != S_OK)
		return -1;

	return 0;
}
//*******************************************************//
//*******************************************************//
int cRenderObject::init()
{
	pFirstChild = NULL;
	pNextSibling = NULL;
	VertexBuffer = NULL;
	Texture = NULL;
	Vertexes = NULL;
	pHistoryBuffer = NULL;
	iNumVertexes = 0;
	bFixedScale2D = false;

	iOrientationMode = 0;
	latitude = 0;
	longitude = 0;
	pr = 0;
	pitch = 0;
	roll = 0;
	yaw = 0;
	px = 0;
	py = 0;
	pz = 0;

	return 0;
}
//*******************************************************//
//*******************************************************//
int cRenderObject::exit()
{
	//release own
	if (VertexBuffer != NULL)
	{
		VertexBuffer->Release();
		VertexBuffer = NULL;
	}
	if (Texture != NULL)
	{
		Texture->Release();
		Texture = NULL;
	}
	if (Vertexes != NULL)
	{
		delete [] Vertexes;
		Vertexes = NULL;
		iNumVertexes = 0;
	}

	//release history buffer if it exists
	if (pHistoryBuffer != NULL)
	{
		delete pHistoryBuffer;
		pHistoryBuffer = NULL;
	}

	cRenderObject* pTrav = pFirstChild;
	cRenderObject* pNext;

	while (pTrav != NULL)
	{
		pNext = pTrav->pNextSibling;

		pTrav->exit();
		delete pTrav;

		pTrav = pNext;			
	}

	return 0;
}
//*******************************************************//
//*******************************************************//
cRenderObject* cRenderObject::add_child()
{
	if (pFirstChild == NULL)
	{
		pFirstChild = new cRenderObject();
		pFirstChild->init();
		return pFirstChild;
	}

	cRenderObject* pTail = pFirstChild;

	while (pTail->pNextSibling != NULL)
		pTail = pTail->pNextSibling;

	cRenderObject* pNew = new cRenderObject();

	pNew->init();
	pTail->pNextSibling = pNew;
	pTail = pNew;

	return pNew;
	
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::init_lights0()
{

    ZeroMemory( &light0, sizeof(light0) );
    light0.Type = D3DLIGHT_POINT;
	light0.Position = eye_vector;//D3DXVECTOR3(0.0f, 10.0f, 20.0f);
	light0.Range = 0.0; //<-- i.e. disabled//300.0f;
	light0.Attenuation0 = 0.0f;
	light0.Attenuation1 = 0.25f;
	light0.Attenuation2 = 0.0f;
    light0.Diffuse.r = light0.Diffuse.g = light0.Diffuse.b = 0.25f;
    Device->SetLight( 0, &light0 );
	Device->LightEnable(0, TRUE);

    // Set light #0 to be a simple, faint grey directional light so 
    // the walls and floor are slightly different shades of grey
    ZeroMemory( &light1, sizeof(light1) );
    light1.Type = D3DLIGHT_DIRECTIONAL;
	light1.Direction = D3DXVECTOR3(0.0f, 10.0f, 20.0f);
	//light1.Range = 3000.0f;
	//light1.Attenuation0 = 0.0f;
	//light1.Attenuation1 = 0.25f;
	//light1.Attenuation2 = 0.0f;
    light1.Diffuse.r = light1.Diffuse.g = light1.Diffuse.b = 1.0f;
    Device->SetLight( 1, &light1 );
	Device->LightEnable(1, TRUE);

	Device->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(128,128,128));

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::build_globe(cRenderObject* pObject)
{
	if (pObject == NULL)
		return -1;

	int divlat = 60;
	int divlong = 60;
	double inclat = 90.0 / divlat;
	double inclong = 360.0 / divlong;

	ZeroMemory( &pObject->Material, sizeof(D3DMATERIAL9));
	pObject->Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pObject->Material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pObject->iNumVertexes = divlat*divlong*12;

	init_vertex_buffer(pObject, pObject->iNumVertexes);

	vertex* vertices = NULL;

	double scale = 10.0;

	pObject->VertexBuffer->Lock(0, 0, (void**)&vertices, D3DLOCK_DISCARD);

	int ct = 0;
	double rlat1 = 0;
	double rlong1 = 0;
	double rlat2 = 0;
	double rlong2 = 0;

	for (double longitude = 0; longitude < 360; longitude += inclong )
	for (double latitude = 0; latitude < 90; latitude += inclat)
	{
		rlat1 = latitude * (3.14159 / 180.0);
		rlat2 = (latitude + inclat) * (3.14159 / 180.0);
		rlong1 = longitude * (3.14159 / 180.0);
		rlong2 = (longitude + inclong) * (3.14159 / 180.0);
		
		if (rlat1 > 1.5707)
			rlat1 = 1.5707;
		if (rlat2 > 1.5707)
			rlat2 = 1.5707;

		vertices[ct].x = scale*sin(rlong1)*cos(rlat1);
		vertices[ct].z = scale*cos(rlong1)*cos(rlat1);
		vertices[ct].y = scale*sin(rlat1);
		vertices[ct].colour = 0xffffffff;
		vertices[ct].tu = 1.0 - rlong1 / 6.283135;
		vertices[ct].tv = 1.0 - (rlat1 / 3.141593) + 0.5;

		vertices[ct+1].x = scale*sin(rlong2)*cos(rlat1);
		vertices[ct+1].z = scale*cos(rlong2)*cos(rlat1);
		vertices[ct+1].y = scale*sin(rlat1);
		vertices[ct+1].colour = 0xffffffff;
		vertices[ct+1].tu = 1.0 - rlong2 / 6.283135;
		vertices[ct+1].tv = 1.0 - (rlat1 / 3.141593) + 0.5;

		vertices[ct+2].x = scale*sin(rlong1)*cos(rlat2);
		vertices[ct+2].z = scale*cos(rlong1)*cos(rlat2);
		vertices[ct+2].y = scale*sin(rlat2);
		vertices[ct+2].colour = 0xffffffff;
		vertices[ct+2].tu = 1.0 - rlong1 / 6.283135;
		vertices[ct+2].tv = 1.0 - (rlat2 / 3.141593) + 0.5;

		vertex normal = compute_normal(vertices[ct], vertices[ct+1], vertices[ct+2]);
		copy_normal(&vertices[ct], &normal);
		copy_normal(&vertices[ct+1], &normal);
		copy_normal(&vertices[ct+2], &normal);

		ct += 3;

		vertices[ct].x = scale*sin(rlong2)*cos(rlat1);
		vertices[ct].z = scale*cos(rlong2)*cos(rlat1);
		vertices[ct].y = scale*sin(rlat1);
		vertices[ct].colour = 0xffffffff;
		vertices[ct].tu = 1.0 - rlong2 / 6.283135;
		vertices[ct].tv = 1.0 - (rlat1 / 3.141593) + 0.5;

		vertices[ct+1].x = scale*sin(rlong2)*cos(rlat2);
		vertices[ct+1].z = scale*cos(rlong2)*cos(rlat2);
		vertices[ct+1].y = scale*sin(rlat2);
		vertices[ct+1].colour = 0xffffffff;
		vertices[ct+1].tu = 1.0 - rlong2 / 6.283135;
		vertices[ct+1].tv = 1.0 - (rlat2 / 3.141593) + 0.5;

		vertices[ct+2].x = scale*sin(rlong1)*cos(rlat2);
		vertices[ct+2].z = scale*cos(rlong1)*cos(rlat2);
		vertices[ct+2].y = scale*sin(rlat2);
		vertices[ct+2].colour = 0xffffffff;
		vertices[ct+2].tu = 1.0 - rlong1 / 6.283135;
		vertices[ct+2].tv = 1.0 - (rlat2 / 3.141593) + 0.5;

		copy_normal(&vertices[ct], &normal);
		copy_normal(&vertices[ct+1], &normal);
		copy_normal(&vertices[ct+2], &normal);

		ct += 3;

		normal.ny *= -1;

		vertices[ct+2].x = scale*sin(rlong1)*cos(rlat1);
		vertices[ct+2].z = scale*cos(rlong1)*cos(rlat1);
		vertices[ct+2].y = -1*scale*sin(rlat1);
		vertices[ct+2].colour = 0xffffffff;
		vertices[ct+2].tu = 1.0 - rlong1 / 6.283135;
		vertices[ct+2].tv = 1.0 - ((rlat1 / 3.141593)*-1) + 0.5;

		vertices[ct+1].x = scale*sin(rlong2)*cos(rlat1);
		vertices[ct+1].z = scale*cos(rlong2)*cos(rlat1);
		vertices[ct+1].y = -1*scale*sin(rlat1);
		vertices[ct+1].colour = 0xffffffff;
		vertices[ct+1].tu = 1.0 - rlong2 / 6.283135;
		vertices[ct+1].tv = 1.0 - ((rlat1 / 3.141593)*-1) + 0.5;

		vertices[ct].x = scale*sin(rlong1)*cos(rlat2);
		vertices[ct].z = scale*cos(rlong1)*cos(rlat2);
		vertices[ct].y = -1*scale*sin(rlat2);
		vertices[ct].colour = 0xffffffff;
		vertices[ct].tu = 1.0 - rlong1 / 6.283135;
		vertices[ct].tv = 1.0 - ((rlat2 / 3.141593)*-1) + 0.5;

		copy_normal(&vertices[ct], &normal);
		copy_normal(&vertices[ct+1], &normal);
		copy_normal(&vertices[ct+2], &normal);

		ct += 3;

		vertices[ct+2].x = scale*sin(rlong2)*cos(rlat1);
		vertices[ct+2].z = scale*cos(rlong2)*cos(rlat1);
		vertices[ct+2].y = -1*scale*sin(rlat1);
		vertices[ct+2].colour = 0xffffffff;
		vertices[ct+2].tu = 1.0 - rlong2 / 6.283135;
		vertices[ct+2].tv = 1.0 - ((rlat1 / 3.141593)*-1) + 0.5;

		vertices[ct+1].x = scale*sin(rlong2)*cos(rlat2);
		vertices[ct+1].z = scale*cos(rlong2)*cos(rlat2);
		vertices[ct+1].y = -1*scale*sin(rlat2);
		vertices[ct+1].colour = 0xffffffff;
		vertices[ct+1].tu = 1.0 - rlong2 / 6.283135;
		vertices[ct+1].tv = 1.0 - ((rlat2 / 3.141593)*-1) + 0.5;

		vertices[ct].x = scale*sin(rlong1)*cos(rlat2);
		vertices[ct].z = scale*cos(rlong1)*cos(rlat2);
		vertices[ct].y = -1*scale*sin(rlat2);
		vertices[ct].colour = 0xffffffff;
		vertices[ct].tu = 1.0 - rlong1 / 6.283135;
		vertices[ct].tv = 1.0 - ((rlat2 / 3.141593)*-1) + 0.5;

		copy_normal(&vertices[ct], &normal);
		copy_normal(&vertices[ct+1], &normal);
		copy_normal(&vertices[ct+2], &normal);

		ct += 3;
		if (ct >= pObject->iNumVertexes)
			break;
	}
	pObject->VertexBuffer->Unlock();

	D3DXCreateTextureFromFile(Device,   //Direct3D Device
							  //L"c:/infile.png",       //File Name
							  L"c:/infile.jpg",       //File Name
                             &pObject->Texture);    //Texture handle

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::init_lights1()
{
    // Set light #0 to be a simple, faint grey directional light so 
    // the walls and floor are slightly different shades of grey
    ZeroMemory( &light0, sizeof(light0) );
    light0.Type = D3DLIGHT_POINT;
	light0.Position = eye_vector;//D3DXVECTOR3(0.0f, 10.0f, 20.0f);
	light0.Range = 300.0f;
	light0.Attenuation0 = 0.0f;
	light0.Attenuation1 = 0.25f;
	light0.Attenuation2 = 0.0f;
    light0.Diffuse.r = light0.Diffuse.g = light0.Diffuse.b = 1.0f;
    Device->SetLight( 0, &light0 );
	Device->LightEnable(0, TRUE);

	Device->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(32,32,32));

	bLight0Changed = false;

	return 0;
}
//*******************************************************//
//*******************************************************//
int cD3D_Device::set_lights()
{
	if (bLight0Changed == false)
		return 0;

	light0.Position = eye_vector;//D3DXVECTOR3(0.0f, 10.0f, 20.0f);
    Device->SetLight( 0, &light0 );

	bLight0Changed = false;

	return 0;
}
//*******************************************************//
//*******************************************************//
//initialize a renderstate for 3D
int cD3D_Device::init_renderstate_3d()
{
	//set vertex format
	Device->SetFVF(vertex_fvf); //this is the "3D" vertex format
	Device->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    Device->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
	Device->SetRenderState( D3DRS_LIGHTING, TRUE);
	Device->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);//D3DZB_USEW); //D3DZB_TRUE);
	//Device->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS);
	Device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	//Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);

	Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
    Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_CURRENT); 

    Device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);//D3DTEXF_NONE); 
    Device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);//D3DTEXF_NONE); 

	// Create a D3DX font object
	D3DXCreateFont( Device, 20, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );
	// Create a D3DX font object
	D3DXCreateFont( Device, 12, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font_small );
	// Create a D3DX font object
	D3DXCreateFont( Device, 36, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font_large );

	return 0;
}
//*******************************************************//
//*******************************************************//
//initialize a renderstate for 2D
int cD3D_Device::init_renderstate_2d()
{
	//set vertex format
	Device->SetFVF(vertex_2d);  //this is the "2d" vertex format
	Device->SetRenderState( D3DRS_LIGHTING, FALSE);
	Device->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE);
	//Device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE);

	Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
    Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
    Device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_CURRENT); 

    Device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);//D3DTEXF_NONE); 
    Device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);//D3DTEXF_NONE); 

	// Create a D3DX font object
	D3DXCreateFont( Device, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );

	return 0;
}
//*******************************************************//
//*******************************************************//

