'Description: TitanPanel is a GUI for displaying simulation data.
'It mainly is just textboxes for displaying the state vector,
'And buttons for doing various things with the simulation.
Public Class Form1
    Declare Function _read8 Lib "Titan4DLL.dll" (ByVal addr As Integer) As Byte
    Declare Function _write8 Lib "Titan4DLL.dll" (ByVal addr As Integer, ByVal data As Byte) As Byte
    Declare Function _read_int Lib "Titan4DLL.dll" (ByVal addr As Integer) As Integer
    Declare Function _write_int Lib "Titan4DLL.dll" (ByVal addr As Integer, ByVal value As Integer) As Integer
    Declare Function _read_double Lib "Titan4DLL.dll" (ByVal addr As Integer) As Double
    Declare Function _write_double Lib "Titan4DLL.dll" (ByVal addr As Integer, ByVal value As Double) As Integer


    Dim VIS_COUNT As Integer

    Dim SIM_PHASE0_LONG As Integer
    Dim SIM_PHASE0_LAT As Integer
    Dim SIM_PHASE0_R As Integer

    Dim SIM_PHASE0_PITCH As Integer
    Dim SIM_PHASE0_ROLL As Integer
    Dim SIM_PHASE0_YAW As Integer

    Dim SIM_COMMAND As Integer
    Dim SIM_TIME As Integer

    Dim VIS_COMMAND As Integer

    Dim SIM_PHASE0_X As Integer
    Dim SIM_PHASE0_Y As Integer
    Dim SIM_PHASE0_Z As Integer

    Dim SIM_PHASE0_VX As Integer
    Dim SIM_PHASE0_VY As Integer
    Dim SIM_PHASE0_VZ As Integer

    Dim SIM_PHASE0_WX As Integer
    Dim SIM_PHASE0_WY As Integer
    Dim SIM_PHASE0_WZ As Integer

    Dim SIM_PHASE0_AIR_DENSITY As Integer
    Dim SIM_PHASE0_STATIC_AIR_TEMPERATURE As Integer
    Dim SIM_PHASE0_STATIC_PRESSURE As Integer

    Dim SIM_PHASE0_ANGLE_OF_ATTACK As Integer
    Dim SIM_PHASE0_CD As Integer
    Dim SIM_PHASE0_CD0 As Integer
    Dim SIM_PHASE0_RHO As Integer

    Dim SIM_PHASE0_Q1 As Integer
    Dim SIM_PHASE0_Q2 As Integer
    Dim SIM_PHASE0_Q3 As Integer
    Dim SIM_PHASE0_Q4 As Integer

    Dim SIM_PHASE0_MASS As Integer

    Dim SIM_PHASE1_STAGE1_X As Integer
    Dim SIM_PHASE1_STAGE1_Y As Integer
    Dim SIM_PHASE1_STAGE1_Z As Integer

    Dim SIM_PHASE1_STAGE1_VX As Integer
    Dim SIM_PHASE1_STAGE1_VY As Integer
    Dim SIM_PHASE1_STAGE1_VZ As Integer

    Dim SIM_PHASE1_STAGE1_PITCH As Integer
    Dim SIM_PHASE1_STAGE1_ROLL As Integer
    Dim SIM_PHASE1_STAGE1_YAW As Integer

    Dim SIM_PHASE1_STAGE1_WX As Integer
    Dim SIM_PHASE1_STAGE1_WY As Integer
    Dim SIM_PHASE1_STAGE1_WZ As Integer

    Dim SIM_PHASE1_STAGE1_R As Integer
    Dim SIM_PHASE1_STAGE1_LONG As Integer
    Dim SIM_PHASE1_STAGE1_LAT As Integer

    Dim SIM_PHASE1_STAGE1_AIR_DENSITY As Integer
    Dim SIM_PHASE1_STAGE1_STATIC_AIR_TEMPERATURE As Integer
    Dim SIM_PHASE1_STAGE1_STATIC_PRESSURE As Integer

    Dim SIM_PHASE1_STAGE1_ANGLE_OF_ATTACK As Integer
    Dim SIM_PHASE1_STAGE1_CD As Integer
    Dim SIM_PHASE1_STAGE1_CD0 As Integer
    Dim SIM_PHASE1_STAGE1_RHO As Integer

    Dim SIM_PHASE1_STAGE1_Q1 As Integer
    Dim SIM_PHASE1_STAGE1_Q2 As Integer
    Dim SIM_PHASE1_STAGE1_Q3 As Integer
    Dim SIM_PHASE1_STAGE1_Q4 As Integer

    Dim SIM_PHASE1_STAGE1_MASS As Integer

    Dim SIM_PHASE1_STAGE2_X As Integer
    Dim SIM_PHASE1_STAGE2_Y As Integer
    Dim SIM_PHASE1_STAGE2_Z As Integer

    Dim SIM_PHASE1_STAGE2_VX As Integer
    Dim SIM_PHASE1_STAGE2_VY As Integer
    Dim SIM_PHASE1_STAGE2_VZ As Integer

    Dim SIM_PHASE1_STAGE2_PITCH As Integer
    Dim SIM_PHASE1_STAGE2_ROLL As Integer
    Dim SIM_PHASE1_STAGE2_YAW As Integer

    Dim SIM_PHASE1_STAGE2_WX As Integer
    Dim SIM_PHASE1_STAGE2_WY As Integer
    Dim SIM_PHASE1_STAGE2_WZ As Integer

    Dim SIM_PHASE1_STAGE2_R As Integer
    Dim SIM_PHASE1_STAGE2_LONG As Integer
    Dim SIM_PHASE1_STAGE2_LAT As Integer

    Dim SIM_PHASE1_STAGE2_AIR_DENSITY As Integer
    Dim SIM_PHASE1_STAGE2_STATIC_AIR_TEMPERATURE As Integer
    Dim SIM_PHASE1_STAGE2_STATIC_PRESSURE As Integer

    Dim SIM_PHASE1_STAGE2_ANGLE_OF_ATTACK As Integer
    Dim SIM_PHASE1_STAGE2_CD As Integer
    Dim SIM_PHASE1_STAGE2_CD0 As Integer
    Dim SIM_PHASE1_STAGE2_RHO As Integer

    Dim SIM_PHASE1_STAGE2_Q1 As Integer
    Dim SIM_PHASE1_STAGE2_Q2 As Integer
    Dim SIM_PHASE1_STAGE2_Q3 As Integer
    Dim SIM_PHASE1_STAGE2_Q4 As Integer

    Dim SIM_PHASE1_STAGE2_MASS As Integer

    Dim SIM_PHASE As Integer

    Dim IC_PX_ECEF As Integer
    Dim IC_PY_ECEF As Integer
    Dim IC_PZ_ECEF As Integer

    Dim IC_VX_ECEF As Integer
    Dim IC_VY_ECEF As Integer
    Dim IC_VZ_ECEF As Integer

    Dim IC_PITCH_RAD As Integer
    Dim IC_ROLL_RAD As Integer
    Dim IC_YAW_RAD As Integer

    Dim IC_WX As Integer
    Dim IC_WY As Integer
    Dim IC_WZ As Integer

    Dim IC_MASS_FUEL_FIRST_STAGE As Integer

    Dim ENGINE_EXTERNAL_CONTROL As Integer

    Dim STAGE1_ENGINE_ON As Integer
    Dim STAGE2_ENGINE_ON As Integer

    Dim SIM_STEP_SIZE As Integer

    Dim deg_to_rad As Double
    Dim rad_to_deg As Double

    Dim run_mode As Integer = 0

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        ENGINE_EXTERNAL_CONTROL = &H70
        SIM_STEP_SIZE = &H80
        SIM_COMMAND = &H90
        VIS_COUNT = &H100

        VIS_COMMAND = &H400

        SIM_PHASE0_LONG = &H110
        SIM_PHASE0_LAT = &H120

        SIM_PHASE0_R = &H130

        SIM_PHASE0_PITCH = &H140
        SIM_PHASE0_ROLL = &H150
        SIM_PHASE0_YAW = &H160

        SIM_TIME = &H170

        SIM_PHASE = &H390

        SIM_PHASE0_X = &H200
        SIM_PHASE0_Y = &H210
        SIM_PHASE0_Z = &H220

        SIM_PHASE0_VX = &H230
        SIM_PHASE0_VY = &H240
        SIM_PHASE0_VZ = &H250

        SIM_PHASE0_WX = &H260
        SIM_PHASE0_WY = &H270
        SIM_PHASE0_WZ = &H280

        SIM_PHASE0_AIR_DENSITY = &H4A0
        SIM_PHASE0_STATIC_AIR_TEMPERATURE = &H4B0
        SIM_PHASE0_STATIC_PRESSURE = &H4C0

        SIM_PHASE0_ANGLE_OF_ATTACK = &H4D0
        SIM_PHASE0_CD = &H4E0
        SIM_PHASE0_CD0 = &H4F0
        SIM_PHASE0_RHO = &H500

        SIM_PHASE0_Q1 = &H510
        SIM_PHASE0_Q2 = &H520
        SIM_PHASE0_Q3 = &H530
        SIM_PHASE0_Q4 = &H540

        SIM_PHASE0_MASS = &H550

        SIM_PHASE1_STAGE1_X = &H1000
        SIM_PHASE1_STAGE1_Y = &H1010
        SIM_PHASE1_STAGE1_Z = &H1020

        SIM_PHASE1_STAGE1_VX = &H1030
        SIM_PHASE1_STAGE1_VY = &H1040
        SIM_PHASE1_STAGE1_VZ = &H1050

        SIM_PHASE1_STAGE1_PITCH = &H1060
        SIM_PHASE1_STAGE1_ROLL = &H1070
        SIM_PHASE1_STAGE1_YAW = &H1080

        SIM_PHASE1_STAGE1_WX = &H1090
        SIM_PHASE1_STAGE1_WY = &H10A0
        SIM_PHASE1_STAGE1_WZ = &H10B0

        SIM_PHASE1_STAGE1_R = &H10C0
        SIM_PHASE1_STAGE1_LONG = &H10D0
        SIM_PHASE1_STAGE1_LAT = &H10E0

        SIM_PHASE1_STAGE1_AIR_DENSITY = &H1150
        SIM_PHASE1_STAGE1_STATIC_AIR_TEMPERATURE = &H1160
        SIM_PHASE1_STAGE1_STATIC_PRESSURE = &H1170

        SIM_PHASE1_STAGE1_ANGLE_OF_ATTACK = &H1180
        SIM_PHASE1_STAGE1_CD = &H1190
        SIM_PHASE1_STAGE1_CD0 = &H11A0
        SIM_PHASE1_STAGE1_RHO = &H11B0

        SIM_PHASE1_STAGE1_Q1 = &H11C0
        SIM_PHASE1_STAGE1_Q2 = &H11D0
        SIM_PHASE1_STAGE1_Q3 = &H11E0
        SIM_PHASE1_STAGE1_Q4 = &H11F0

        SIM_PHASE1_STAGE1_MASS = &H1200

        SIM_PHASE1_STAGE2_X = &H3000
        SIM_PHASE1_STAGE2_Y = &H3010
        SIM_PHASE1_STAGE2_Z = &H3020

        SIM_PHASE1_STAGE2_VX = &H3030
        SIM_PHASE1_STAGE2_VY = &H3040
        SIM_PHASE1_STAGE2_VZ = &H3050

        SIM_PHASE1_STAGE2_PITCH = &H3060
        SIM_PHASE1_STAGE2_ROLL = &H3070
        SIM_PHASE1_STAGE2_YAW = &H3080

        SIM_PHASE1_STAGE2_WX = &H3090
        SIM_PHASE1_STAGE2_WY = &H30A0
        SIM_PHASE1_STAGE2_WZ = &H30B0

        SIM_PHASE1_STAGE2_R = &H30C0
        SIM_PHASE1_STAGE2_LONG = &H30D0
        SIM_PHASE1_STAGE2_LAT = &H30E0

        SIM_PHASE1_STAGE2_AIR_DENSITY = &H3150
        SIM_PHASE1_STAGE2_STATIC_AIR_TEMPERATURE = &H3160
        SIM_PHASE1_STAGE2_STATIC_PRESSURE = &H3170

        SIM_PHASE1_STAGE2_ANGLE_OF_ATTACK = &H3180
        SIM_PHASE1_STAGE2_CD = &H3190
        SIM_PHASE1_STAGE2_CD0 = &H31A0
        SIM_PHASE1_STAGE2_RHO = &H31B0

        SIM_PHASE1_STAGE2_Q1 = &H31C0
        SIM_PHASE1_STAGE2_Q2 = &H31D0
        SIM_PHASE1_STAGE2_Q3 = &H31E0
        SIM_PHASE1_STAGE2_Q4 = &H31F0

        SIM_PHASE1_STAGE2_MASS = &H3200

        IC_PX_ECEF = &H5000
        IC_PY_ECEF = &H5010
        IC_PZ_ECEF = &H5020

        IC_VX_ECEF = &H5030
        IC_VY_ECEF = &H5040
        IC_VZ_ECEF = &H5050

        IC_PITCH_RAD = &H5060
        IC_ROLL_RAD = &H5070
        IC_YAW_RAD = &H5080

        IC_WX = &H5090
        IC_WY = &H50A0
        IC_WZ = &H50B0

        IC_MASS_FUEL_FIRST_STAGE = &H50C0

        STAGE1_ENGINE_ON = &H5100
        STAGE2_ENGINE_ON = &H5110

        deg_to_rad = 3.14159 / 180.0
        rad_to_deg = 1.0 / deg_to_rad

    End Sub

    Public Sub download_coords_phase0()

        longbox_00.Text = (_read_double(SIM_PHASE0_LONG) * rad_to_deg).ToString("g8")
        latbox_00.Text = (_read_double(SIM_PHASE0_LAT) * rad_to_deg).ToString("g8")
        rbox_00.Text = _read_double(SIM_PHASE0_R).ToString("g12")
        alt_box_00.Text = (_read_double(SIM_PHASE0_R) - 6378.1).ToString("g8")

        pitchbox_00.Text = (_read_double(SIM_PHASE0_PITCH) * rad_to_deg).ToString("g6")
        rollbox_00.Text = (_read_double(SIM_PHASE0_ROLL) * rad_to_deg).ToString("g6")
        yawbox_00.Text = (_read_double(SIM_PHASE0_YAW) * rad_to_deg).ToString("g6")

        q1_box_00.Text = _read_double(SIM_PHASE0_Q1).ToString("g6")
        q2_box_00.Text = _read_double(SIM_PHASE0_Q2).ToString("g6")
        q3_box_00.Text = _read_double(SIM_PHASE0_Q3).ToString("g6")
        q4_box_00.Text = _read_double(SIM_PHASE0_Q4).ToString("g6")

        rho_box_00.Text = _read_double(SIM_PHASE0_AIR_DENSITY).ToString("g6")
        aoa_box_00.Text = _read_double(SIM_PHASE0_ANGLE_OF_ATTACK).ToString("g6")
        cd_box_00.Text = _read_double(SIM_PHASE0_CD).ToString("g6")
        cd0_box_00.Text = _read_double(SIM_PHASE0_CD0).ToString("g6")

        Dim px As Double = _read_double(SIM_PHASE0_X)
        Dim py As Double = _read_double(SIM_PHASE0_Y)
        Dim pz As Double = _read_double(SIM_PHASE0_Z)

        x_box_00.Text = px.ToString("g8")
        y_box_00.Text = py.ToString("g8")
        z_box_00.Text = pz.ToString("g8")

        Dim vx As Double = _read_double(SIM_PHASE0_VX)
        Dim vy As Double = _read_double(SIM_PHASE0_VY)
        Dim vz As Double = _read_double(SIM_PHASE0_VZ)
        Dim v As Double = 0

        If (vx <> 0) And (vy <> 0) And (vz <> 0) Then
            v = Math.Sqrt(vx * vx + vy * vy + vz * vz)
        End If

        v_box_00.Text = v.ToString("g6")
        vx_box_00.Text = vx.ToString("g6")
        vy_box_00.Text = vy.ToString("g6")
        vz_box_00.Text = vz.ToString("g6")

        Dim wx As Double = _read_double(SIM_PHASE0_WX)
        Dim wy As Double = _read_double(SIM_PHASE0_WY)
        Dim wz As Double = _read_double(SIM_PHASE0_WZ)
        Dim w As Double = 0

        If (wx <> 0) And (wy <> 0) And (wz <> 0) Then
            w = Math.Sqrt(wx * wx + wy * wy + wz * wz)
        End If

        w_box_00.Text = w.ToString("g6")
        wx_box_00.Text = wx.ToString("g6")
        wy_box_00.Text = wy.ToString("g6")
        wz_box_00.Text = wz.ToString("g6")

        mass_box_00.Text = _read_double(SIM_PHASE0_MASS).ToString("g6")

    End Sub
    Public Sub download_coords_phase1()

        longbox_01.Text = (_read_double(SIM_PHASE1_STAGE2_LONG) * rad_to_deg).ToString("g8")
        latbox_01.Text = (_read_double(SIM_PHASE1_STAGE2_LAT) * rad_to_deg).ToString("g8")
        rbox_01.Text = _read_double(SIM_PHASE1_STAGE2_R).ToString("g12")
        alt_box_01.Text = (_read_double(SIM_PHASE1_STAGE2_R) - 6378.1).ToString("g8")

        pitchbox_01.Text = (_read_double(SIM_PHASE1_STAGE2_PITCH) * rad_to_deg).ToString("g6")
        rollbox_01.Text = (_read_double(SIM_PHASE1_STAGE2_ROLL) * rad_to_deg).ToString("g6")
        yawbox_01.Text = (_read_double(SIM_PHASE1_STAGE2_YAW) * rad_to_deg).ToString("g6")

        q1_box_01.Text = _read_double(SIM_PHASE1_STAGE2_Q1).ToString("g6")
        q2_box_01.Text = _read_double(SIM_PHASE1_STAGE2_Q2).ToString("g6")
        q3_box_01.Text = _read_double(SIM_PHASE1_STAGE2_Q3).ToString("g6")
        q4_box_01.Text = _read_double(SIM_PHASE1_STAGE2_Q4).ToString("g6")

        rho_box_01.Text = _read_double(SIM_PHASE1_STAGE2_AIR_DENSITY).ToString("g6")
        aoa_box_01.Text = _read_double(SIM_PHASE1_STAGE2_ANGLE_OF_ATTACK).ToString("g6")
        cd_box_01.Text = _read_double(SIM_PHASE1_STAGE2_CD).ToString("g6")
        cd0_box_01.Text = _read_double(SIM_PHASE1_STAGE2_CD0).ToString("g6")

        Dim px As Double = _read_double(SIM_PHASE1_STAGE2_X)
        Dim py As Double = _read_double(SIM_PHASE1_STAGE2_Y)
        Dim pz As Double = _read_double(SIM_PHASE1_STAGE2_Z)

        x_box_01.Text = px.ToString("g8")
        y_box_01.Text = py.ToString("g8")
        z_box_01.Text = pz.ToString("g8")

        Dim vx As Double = _read_double(SIM_PHASE1_STAGE2_VX)
        Dim vy As Double = _read_double(SIM_PHASE1_STAGE2_VY)
        Dim vz As Double = _read_double(SIM_PHASE1_STAGE2_VZ)
        Dim v As Double = 0

        If (vx <> 0) And (vy <> 0) And (vz <> 0) Then
            v = Math.Sqrt(vx * vx + vy * vy + vz * vz)
        End If

        v_box_01.Text = v.ToString("g6")
        vx_box_01.Text = vx.ToString("g6")
        vy_box_01.Text = vy.ToString("g6")
        vz_box_01.Text = vz.ToString("g6")

        Dim wx As Double = _read_double(SIM_PHASE1_STAGE2_WX)
        Dim wy As Double = _read_double(SIM_PHASE1_STAGE2_WY)
        Dim wz As Double = _read_double(SIM_PHASE1_STAGE2_WZ)
        Dim w As Double = 0

        If (wx <> 0) And (wy <> 0) And (wz <> 0) Then
            w = Math.Sqrt(wx * wx + wy * wy + wz * wz)
        End If

        w_box_01.Text = w.ToString("g6")
        wx_box_01.Text = wx.ToString("g6")
        wy_box_01.Text = wy.ToString("g6")
        wz_box_01.Text = wz.ToString("g6")

        mass_box_01.Text = _read_double(SIM_PHASE1_STAGE2_MASS).ToString("g6")

        'no go back and do it for the first stage

        longbox_00.Text = (_read_double(SIM_PHASE1_STAGE1_LONG) * rad_to_deg).ToString("g8")
        latbox_00.Text = (_read_double(SIM_PHASE1_STAGE1_LAT) * rad_to_deg).ToString("g8")
        rbox_00.Text = _read_double(SIM_PHASE1_STAGE1_R).ToString("g12")
        alt_box_00.Text = (_read_double(SIM_PHASE1_STAGE1_R) - 6378.1).ToString("g8")

        pitchbox_00.Text = (_read_double(SIM_PHASE1_STAGE1_PITCH) * rad_to_deg).ToString("g6")
        rollbox_00.Text = (_read_double(SIM_PHASE1_STAGE1_ROLL) * rad_to_deg).ToString("g6")
        yawbox_00.Text = (_read_double(SIM_PHASE1_STAGE1_YAW) * rad_to_deg).ToString("g6")

        q1_box_00.Text = _read_double(SIM_PHASE1_STAGE1_Q1).ToString("g6")
        q2_box_00.Text = _read_double(SIM_PHASE1_STAGE1_Q2).ToString("g6")
        q3_box_00.Text = _read_double(SIM_PHASE1_STAGE1_Q3).ToString("g6")
        q4_box_00.Text = _read_double(SIM_PHASE1_STAGE1_Q4).ToString("g6")

        rho_box_00.Text = _read_double(SIM_PHASE1_STAGE1_AIR_DENSITY).ToString("g6")
        aoa_box_00.Text = _read_double(SIM_PHASE1_STAGE1_ANGLE_OF_ATTACK).ToString("g6")
        cd_box_00.Text = _read_double(SIM_PHASE1_STAGE1_CD).ToString("g6")
        cd0_box_00.Text = _read_double(SIM_PHASE1_STAGE1_CD0).ToString("g6")

        px = _read_double(SIM_PHASE1_STAGE1_X)
        py = _read_double(SIM_PHASE1_STAGE1_Y)
        pz = _read_double(SIM_PHASE1_STAGE1_Z)

        x_box_00.Text = px.ToString("g8")
        y_box_00.Text = py.ToString("g8")
        z_box_00.Text = pz.ToString("g8")

        vx = _read_double(SIM_PHASE1_STAGE1_VX)
        vy = _read_double(SIM_PHASE1_STAGE1_VY)
        vz = _read_double(SIM_PHASE1_STAGE1_VZ)
        v = 0

        If (vx <> 0) And (vy <> 0) And (vz <> 0) Then
            v = Math.Sqrt(vx * vx + vy * vy + vz * vz)
        End If

        v_box_00.Text = v.ToString("g6")
        vx_box_00.Text = vx.ToString("g6")
        vy_box_00.Text = vy.ToString("g6")
        vz_box_00.Text = vz.ToString("g6")

        wx = _read_double(SIM_PHASE1_STAGE1_WX)
        wy = _read_double(SIM_PHASE1_STAGE1_WY)
        wz = _read_double(SIM_PHASE1_STAGE1_WZ)
        w = 0

        If (wx <> 0) And (wy <> 0) And (wz <> 0) Then
            w = Math.Sqrt(wx * wx + wy * wy + wz * wz)
        End If

        w_box_00.Text = w.ToString("g6")
        wx_box_00.Text = wx.ToString("g6")
        wy_box_00.Text = wy.ToString("g6")
        wz_box_00.Text = wz.ToString("g6")

        mass_box_00.Text = _read_double(SIM_PHASE1_STAGE1_MASS).ToString("g6")

    End Sub
    Public Sub set_lights()

        Dim stage1_engine_firing As Boolean = False
        Dim stage2_engine_firing As Boolean = False

        If _read_int(SIM_PHASE) = 0 Then
            If _read_int(STAGE1_ENGINE_ON) <> 0 Then
                stage1_engine_firing = True
            End If
        End If

        If _read_int(SIM_PHASE) = 1 Then
            If _read_int(STAGE2_ENGINE_ON) <> 0 Then
                stage2_engine_firing = True
            End If
        End If

        If stage1_engine_firing Then
            If FirstStageFiringLabel.BackColor <> Color.Green Then
                FirstStageFiringLabel.BackColor = Color.Green
            End If
        Else
            If FirstStageFiringLabel.BackColor <> Color.Gray Then
                FirstStageFiringLabel.BackColor = Color.Gray
            End If
        End If

        If stage2_engine_firing Then
            If SecondStageFiringLabel.BackColor <> Color.Green Then
                SecondStageFiringLabel.BackColor = Color.Green
            End If
        Else
            If SecondStageFiringLabel.BackColor <> Color.Gray Then
                SecondStageFiringLabel.BackColor = Color.Gray
            End If
        End If


    End Sub

    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        TextBox1.Text = _read_int(&H100).ToString

        Dim phase As Integer = _read_int(SIM_PHASE)
        Dim run_time As Double = _read_double(SIM_TIME)
        Dim step_size As Double = _read_double(SIM_STEP_SIZE)
        Dim engine_ext_control As Integer = _read_int(ENGINE_EXTERNAL_CONTROL)

        If phase = 0 Then
            download_coords_phase0()
        End If
        If phase = 1 Then
            download_coords_phase1()
        End If

        set_lights()

        sim_time_box.Text = _read_double(SIM_TIME).ToString("g8")
        step_size_box.Text = CInt(step_size * 1000.0).ToString()

        If engine_ext_control = 0 Then
            LabelEngineControl.Text = "Engine Control: Internal"
        Else
            LabelEngineControl.Text = "Engine Control: External"
        End If

        If run_mode = 1 Then
            _write_int(SIM_COMMAND, 2)
            If (phase = 0) And (run_time > 169.0) Then
                If sep_checkbox.Checked Then
                    _write_int(SIM_COMMAND, 100)
                    'flip over to the 2nd page
                    TabControl1.SelectTab(1)
                End If
            End If
        End If

    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        _write_int(SIM_COMMAND, 2)
    End Sub

    Private Sub Button4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button4.Click
        'for now the IC button defaults to cape canaveral
        _write_double(IC_PX_ECEF, 919767)
        _write_double(IC_PY_ECEF, -5530997)
        _write_double(IC_PZ_ECEF, 3040108)
        _write_double(IC_VX_ECEF, 0)
        _write_double(IC_VY_ECEF, 0)
        _write_double(IC_VZ_ECEF, 0)
        _write_double(IC_WX, 0)
        _write_double(IC_WY, 0)
        _write_double(IC_WZ, 0)
        _write_double(IC_PITCH_RAD, 3.14159265 / 2.0)
        _write_double(IC_ROLL_RAD, 0)
        _write_double(IC_YAW_RAD, 0)
        _write_double(IC_MASS_FUEL_FIRST_STAGE, 0) 'use this to force defaults
        _write_double(SIM_STEP_SIZE, 0.05)
        _write_double(ENGINE_EXTERNAL_CONTROL, 0)

        _write_int(SIM_COMMAND, 1)
        _write_int(VIS_COMMAND, 2)
    End Sub

    Private Sub Button5_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button5.Click
        _write_int(SIM_COMMAND, 3)
    End Sub

    Private Sub Button6_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button6.Click
        run_mode = 1
    End Sub

    Private Sub Button7_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button7.Click
        run_mode = 0
    End Sub

    Private Sub GroupBox4_Enter(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Aerodynamics.Enter

    End Sub

    Private Sub TextBox3_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cd_box_01.TextChanged

    End Sub

    Private Sub Timer2_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer2.Tick

    End Sub

    Private Sub Label28_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Label28.Click

    End Sub

    Private Sub Label27_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Label27.Click

    End Sub

    Private Sub cd0_box_00_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cd0_box_00.TextChanged

    End Sub

    Private Sub cd_box_00_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cd_box_00.TextChanged

    End Sub

    Private Sub Label42_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Label42.Click

    End Sub
End Class
