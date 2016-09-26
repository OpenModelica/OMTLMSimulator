package TLM
  package TLM_Functions
    function TLMSetMotion
      input String name "Name of the interface";
      input Real time_in "Simulation time";
      input Real R[3] "Position vector - can be neglected (set {0,0,0}) if you do not use 3D modeling and verification";
      input Real A[3, 3] "Rotation matrix - can be neglected (set {1,0,0}{0,1,0}{0,0,1}) if you do not use 3D modeling and verification";
      input Real v[3] "Velocity";
      input Real w[3] "Angular velocity";
    
      external "C" set_tlm_motion(name, time_in, R, A, v, w) annotation(Include = "#include<tlmforce.h>", Library = "tlmmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMSetMotion;

    function TLMGetForce
      input String name "Name of the interface";
      input Real time_in "Simulation time";
      input Real r[3] "Position Vector";
      input Real A[3, 3] "Rotation matrix";
      input Real v[3] "Velocity";
      input Real w[3] "Angular velocity";
      output Real f[3] "Force vector";
      output Real t[3] "Torque/moment vector";
    
      external "C" calc_tlm_force(name, time_in, r, A, v, w, f, t) annotation(Include = "#include<tlmforce.h>", Library = "tlmmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMGetForce;

    function TLMGetDelay
      input String name "Name of the interface";
      output Real TLMdelay "The TLM delay for the secific interface";
    
      external "C" TLMdelay = get_tlm_delay(name) annotation(Include = "#include<tlmforce.h>", Library = "tlmmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMGetDelay;

    function TLMSetDebugMode
      input Boolean DebugFlg "The TLM debug flag, enable or disable debug mode";
    
      external "C" set_debug_mode(DebugFlg) annotation(Include = "#include<tlmforce.h>", Library = "tlmmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMSetDebugMode;
    
    function TLMInitialize
      input String name "Name of the interface";

      external "C" initialize_interface(name) annotation(Include = "#include<tlmforce.h>", Library = "tlmmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics={  Rectangle(visible = true, fillColor = {255, 85, 0},
      fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMInitialize;
    
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Functions;

  package TLM_Interface_1D
    model TLMTorque1D
      Modelica.Mechanics.Rotational.Interfaces.Flange_b flange_b annotation(Placement(visible = true, transformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      constant Real vel[3] = zeros(3);
      Real w[3](start = zeros(3));
      constant Real r[3] = zeros(3);
      constant Real A[3, 3] = identity(3);
      Real f[3](start = zeros(3));
      Real torque[3](start = zeros(3));
      parameter Real tlmDelay = TLMGetDelay(interfaceName);
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
      TLM_Functions.TLMInitialize(interfaceName);
      TLMSetDebugMode(debugFlg);
    equation
      w[1] = der(flange_b.phi);
      w[2] = 0.0;
      w[3] = 0.0;
      flange_b.tau = torque[1];
    algorithm
      (f, torque) := TLMGetForce(interfaceName, time, TLMTorque1D.r, TLMTorque1D.A, TLMTorque1D.vel, w);
    algorithm
      when sample(0.0, tlmDelay / 1.0) then
        TLMSetMotion(interfaceName, time, TLMTorque1D.r, TLMTorque1D.A, TLMTorque1D.vel, w);
      end when;
      annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Line(visible = true, points = {{-63.226, -2.581}, {-28.387, -2.581}}, color = {170, 0, 127}, thickness = 5), Rectangle(visible = true, lineColor = {128, 0, 128}, fillColor = {0, 0, 255}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.90300000000001, 42.581}}, radius = 20), Text(visible = true, origin = {35, -1.626}, textColor = {255, 255, 255}, extent = {{-55, -26.837}, {55, 26.837}}, textString = "1D Torque")}));
    end TLMTorque1D;

    model TLMForce1D
      Modelica.Mechanics.Translational.Interfaces.Flange_b flange_b annotation(Placement(visible = true, transformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      Real vel[3](start = zeros(3));
      constant Real w[3] = zeros(3);
      constant Real r[3] = zeros(3);
      constant Real A[3, 3] = identity(3);
      Real force[3](start = zeros(3));
      Real torque[3](start = zeros(3));
      parameter Real tlmDelay = TLMGetDelay(interfaceName);
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
      TLM_Functions.TLMInitialize(interfaceName);
      TLMSetDebugMode(debugFlg);
    equation
      vel[1] = der(flange_b.s);
      vel[2] = 0.0;
      vel[3] = 0.0;
      flange_b.f = force[1];
    algorithm
      (force, torque) := TLMGetForce(interfaceName, time, TLMForce1D.r, TLMForce1D.A, vel, TLMForce1D.w);
    algorithm
      when sample(tlmDelay, tlmDelay / 1.0) then
        TLMSetMotion(interfaceName, time, TLMForce1D.r, TLMForce1D.A, vel, TLMForce1D.w);
      end when;
      annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, lineColor = {128, 0, 128}, fillColor = {128, 0, 128}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.90300000000001, 42.581}}, radius = 20), Line(visible = true, points = {{-63.226, -2.581}, {-28.387, -2.581}}, color = {128, 0, 128}, thickness = 5), Text(visible = true, origin = {33.696, 0}, textColor = {255, 255, 255}, extent = {{-53.696, -30}, {53.696, 30}}, textString = "1D Force")}));
    end TLMForce1D;
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {170, 9, 95}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {0, 2.861}, textColor = {255, 255, 255}, extent = {{-84.78700000000001, -62.861}, {84.78700000000001, 62.861}}, textString = "1D")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Interface_1D;

  package TLM_Interface_3D
    model TLMInterface3D
      import F = Modelica.Mechanics.MultiBody.Frames;
      import M = Modelica.Mechanics.MultiBody.Frames.TransformationMatrices;
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation(Placement(visible = true, transformation(origin = {-67.9907, -3.03738}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-67.9907, -3.03738}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      Real v[3];
      Real w[3];
      Real f[3](start = zeros(3));
      Real t[3](start = zeros(3));
      Real r[3];
      Real A[3, 3];
      Real AT[3, 3];
      parameter Real tlmDelay = TLM_Functions.TLMGetDelay(interfaceName);
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
      TLM_Functions.TLMInitialize(interfaceName);
      TLM_Functions.TLMSetDebugMode(debugFlg);
    equation
      //
      // World to frame transformation matrix
      A = frame_a.R.T;
      //
      // Frame to world transformation matrix
      AT = transpose(frame_a.R.T);
      //
      // Transform motion into world system
      r = frame_a.r_0;
      v = der(frame_a.r_0);
      w = M.resolve2(AT, frame_a.R.w);
      // w = frame_a.R.w;
      //
      // Transform force and moment into local system
      frame_a.f = M.resolve2(A, f);
      frame_a.t = M.resolve2(A, t);
    algorithm
      (f, t) := TLM_Functions.TLMGetForce(interfaceName, time, r, A, v, w);
    algorithm
      when sample(tlmDelay, tlmDelay / 1.0) then
        TLM_Functions.TLMSetMotion(interfaceName, time, r, A, pre(v), pre(w));
      end when;
      annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, lineColor = {255, 255, 255}, fillColor = {0, 0, 255}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.90300000000001, 42.581}}, radius = 20), Line(visible = true, points = {{-63.226, -2.581}, {-28.387, -2.581}}, color = {0, 0, 255}, thickness = 5), Text(visible = true, origin = {35.194, -1.225}, textColor = {255, 255, 255}, extent = {{-59.815, -21.225}, {59.815, 21.225}}, textString = "3D Force and Torque")}));
    end TLMInterface3D;
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {0, 170, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {-3.808, 3.341}, textColor = {255, 255, 255}, extent = {{-78.774, -56.659}, {78.774, 56.659}}, textString = "3D")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Interface_3D;

  package TLM_Sensors
    model TLMSensor
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      Real v[3] = {0, 0, 0};
      Real w[3] = {0, 0, 0};
      Real f[3](start = {0, 0, 0});
      Real t[3];
      Real r[3] = {0, 0, 0};
      Real A[3, 3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
      parameter Real tlmDelay = TLMGetDelay(interfaceName);
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
      TLM_Functions.TLMInitialize(interfaceName);
      TLMSetDebugMode(debugFlg);
    algorithm
      (f, t) := TLMGetForce(interfaceName, time, r, A, v, w);
    algorithm
      when sample(tlmDelay, tlmDelay / 1.0) then
        TLMSetMotion(interfaceName, time, r, A, v, w);
      end when;
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, origin = {-10.651, -8.377000000000001}, lineColor = {0, 0, 255}, fillColor = {128, 0, 0}, fillPattern = FillPattern.Solid, extent = {{-89.349, -91.623}, {110.651, 108.377}}, radius = 20), Text(visible = true, origin = {-15.021, -3.686}, textColor = {255, 255, 255}, extent = {{-81.61199999999999, -54.575}, {111.654, 66.09099999999999}}, textString = "S")}), Diagram(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, lineColor = {0, 0, 255}, fillColor = {255, 255, 255}, fillPattern = FillPattern.Solid, extent = {{-74.476, -43.157}, {92.232, 51.048}}), Text(visible = true, origin = {-6.228, -0.744}, textColor = {0, 0, 255}, extent = {{-50.313, -33.645}, {68.833, 40.744}}, textString = "TLM Sensor")}));
    end TLMSensor;
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, origin = {-10.651, -8.377000000000001}, lineColor = {0, 0, 255}, fillColor = {128, 0, 0}, fillPattern = FillPattern.Solid, extent = {{-89.349, -91.623}, {110.651, 108.377}}, radius = 20), Text(visible = true, origin = {-15.021, -3.686}, textColor = {255, 255, 255}, extent = {{-81.61199999999999, -54.575}, {111.654, 66.09099999999999}}, textString = "S")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Sensors;
  annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {0, 0, 255}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, textColor = {255, 255, 255}, extent = {{-86.59099999999999, -54.721}, {86.59099999999999, 54.721}}, textString = "TLM")}), uses(Modelica(version = "3.2.1")));
end TLM;
