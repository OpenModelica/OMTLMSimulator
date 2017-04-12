package TLM
  package TLM_Functions
    class TLMPlugin
      extends ExternalObject;

      function constructor
        output TLMPlugin tlmPlugin;
      
        external "C" tlmPlugin = initialize_TLM() annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      end constructor;

      function destructor
        input TLMPlugin tlmPlugin;
      
        external "C" deinitialize_TLM(tlmPlugin) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      end destructor;
    end TLMPlugin;

    function TLMRegisterInterface
      input TLMPlugin tlmPlugin;
      input String name "Name of the interface";
      input String causality "Input/Output/Bidirectional";
      input Integer dimensions "Dimension of the interface";
      input String domain "Domain of the interface";
      
      external "C" register_tlm_interface(tlmPlugin, name, causality, dimensions, domain) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library", __OpenModelica_Impure = true);
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMRegisterInterface;




  
  
    function TLMSetMotion
      input TLMPlugin tlmPlugin;
      input String name "Name of the interface";
      input Real time_in "Simulation time";
      input Real R[3] "Position vector - can be neglected (set {0,0,0}) if you do not use 3D modeling and verification";
      input Real A[3, 3] "Rotation matrix - can be neglected (set {1,0,0}{0,1,0}{0,0,1}) if you do not use 3D modeling and verification";
      input Real v[3] "Velocity";
      input Real w[3] "Angular velocity";
    
      external "C" set_tlm_motion(tlmPlugin, name, time_in, R, A, v, w) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(origin = {-50, -50}, lineColor = {85, 255, 127}, fillColor = {0, 170, 255}, fillPattern = FillPattern.Sphere, extent = {{-35, 90}, {50, 10}}), Text(origin = {-40, 0}, extent = {{-25, 25}, {25, -25}}, textString = "TLM"), Line(origin = {50, 0}, points = {{-50, 0}, {40, 0}}, thickness = 10, arrow = {Arrow.Filled, Arrow.None}, arrowSize = 32)}));
    end TLMSetMotion;

    function TLMGetForce
      input TLMPlugin tlmPlugin;
      input String name "Name of the interface";
      input Real time_in "Simulation time";
      input Real r[3] "Position Vector";
      input Real A[3, 3] "Rotation matrix";
      input Real v[3] "Velocity";
      input Real w[3] "Angular velocity";
      output Real f[3] "Force vector";
      output Real t[3] "Torque/moment vector";
    
      external "C" calc_tlm_force(tlmPlugin, name, time_in, r, A, v, w, f, t) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMGetForce;

    function TLMGetForce1D
      input TLMPlugin tlmPlugin;
      input String name "Name of the interface";
      input Real time_in "Simulation time";
      input Real x "Position";
      input Real v "Velocity";
      output Real f "Force";
    
      external "C" calc_tlm_force_1d(tlmPlugin, name, time_in, x, v, f) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMGetForce1D;

    function TLMGetTorque1D
      input TLMPlugin tlmPlugin;
      input String name "Name of the interface";
      input Real time_in "Simulation time";
      input Real phi "Position";
      input Real w "Velocity";
      output Real t "Force";
    
      external "C" calc_tlm_torque_1d(tlmPlugin, name, time_in, phi, w, t) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMGetTorque1D;
    
    function TLMGetInputValue
      input TLMPlugin tlmPlugin;
      input String name "Name of the interface";
      input Real time_in "Simulation time";
      output Real x "Value";
  
      external "C" get_tlm_input_value(tlmPlugin, name, time_in, x) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMGetInputValue;

  
    function TLMSetOutputValue
      input TLMPlugin tlmPlugin;
      input String name "Name of the interface";
      input Real time_in "Simulation time";
      input Real x "Value";
  
      external "C" set_tlm_output_value(tlmPlugin, name, time_in, x) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMSetOutputValue;

  
    function TLMGetDelay
      input String name "Name of the interface";
      output Real TLMdelay "The TLM delay for the secific interface";
    
      external "C" TLMdelay = get_tlm_delay(name) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library");
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}), __OpenModelica_Impure = true);
    end TLMGetDelay;

    function TLMSetDebugMode
      input Boolean DebugFlg "The TLM debug flag, enable or disable debug mode";
    
      external "C" set_debug_mode(DebugFlg) annotation(Include = "#include<tlmforce.h>", Library = "tlmopenmodelica", IncludeDirectory = "modelica://TLM/Resources/Include", LibraryDirectory = "modelica://TLM/Resources/Library", __OpenModelica_Impure = true);
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}));
    end TLMSetDebugMode;
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {255, 85, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {3.13, 5}, textColor = {255, 255, 255}, extent = {{-66.87, -65}, {66.87, 65}}, textString = "F")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Functions;


package TLM_Interface_Signal
    model TLMInput
      TLM_Functions.TLMPlugin tlmPlugin = TLM_Functions.TLMPlugin();
      Modelica.Blocks.Interfaces.RealOutput y annotation(Placement(visible = true, transformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      Real tlmDelay = TLM_Functions.TLMGetDelay(interfaceName);
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
      TLM_Functions.TLMSetDebugMode(debugFlg);  
    algorithm
      TLM_Functions.TLMSetDebugMode(debugFlg);
      y := TLM_Functions.TLMGetInputValue(tlmPlugin, interfaceName, time);
      annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Line(visible = true, points = {{-63.226, -2.581}, {-28.387, -2.581}}, color = {170, 0, 127}, thickness = 5), Rectangle(visible = true, lineColor = {128, 0, 128}, fillColor = {0, 0, 255}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.90300000000001, 42.581}}, radius = 20), Text(visible = true, origin = {35, -1.626}, textColor = {255, 255, 255}, extent = {{-55, -26.837}, {55, 26.837}}, textString = "1D Torque")}));
    end TLMInput;










model TLMOutput
  TLM_Functions.TLMPlugin tlmPlugin = TLM_Functions.TLMPlugin();
  Modelica.Blocks.Interfaces.RealInput u annotation(Placement(visible = true, transformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  parameter String interfaceName = "tlm";
  parameter Boolean debugFlg = false;
  Real tlmDelay = TLM_Functions.TLMGetDelay(interfaceName);
initial algorithm
  assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
  TLM_Functions.TLMSetDebugMode(debugFlg);
algorithm
  TLM_Functions.TLMSetDebugMode(debugFlg);
  TLM_Functions.TLMSetOutputValue(tlmPlugin, interfaceName, time, u);
  annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, lineColor = {128, 0, 128}, fillColor = {128, 0, 128}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.90300000000001, 42.581}}, radius = 20), Line(visible = true, points = {{-63.226, -2.581}, {-28.387, -2.581}}, color = {128, 0, 128}, thickness = 5), Text(visible = true, origin = {33.696, 0}, textColor = {255, 255, 255}, extent = {{-53.696, -30}, {53.696, 30}}, textString = "1D Force")}));
end TLMOutput;







    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {170, 9, 95}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {0, 2.861}, textColor = {255, 255, 255}, extent = {{-84.78700000000001, -62.861}, {84.78700000000001, 62.861}}, textString = "1D")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Interface_Signal;


  package TLM_Interface_1D
    model TLMTorque1D
      TLM_Functions.TLMPlugin tlmPlugin = TLM_Functions.TLMPlugin();
      Modelica.Mechanics.Rotational.Interfaces.Flange_b flange_b annotation(Placement(visible = true, transformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      Real w(start = 0);
      Real t(start = 0);
      Real tlmDelay = TLM_Functions.TLMGetDelay(interfaceName);
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
      TLM_Functions.TLMSetDebugMode(debugFlg);
    equation
      w = der(flange_b.phi);
      flange_b.tau = t;
    algorithm
      t := TLM_Functions.TLMGetTorque1D(tlmPlugin, interfaceName, time, flange_b.phi, w);
      annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Line(visible = true, points = {{-63.226, -2.581}, {-28.387, -2.581}}, color = {170, 0, 127}, thickness = 5), Rectangle(visible = true, lineColor = {128, 0, 128}, fillColor = {0, 0, 255}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.90300000000001, 42.581}}, radius = 20), Text(visible = true, origin = {35, -1.626}, textColor = {255, 255, 255}, extent = {{-55, -26.837}, {55, 26.837}}, textString = "1D Torque")}));
    end TLMTorque1D;

model TLMForce1D
  TLM_Functions.TLMPlugin tlmPlugin = TLM_Functions.TLMPlugin();
  Modelica.Mechanics.Translational.Interfaces.Flange_b flange_b annotation(Placement(visible = true, transformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-76.77419999999999, -1.93548}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
  parameter String interfaceName = "tlm";
  parameter Boolean debugFlg = false;
  Real v(start = 0);
  Real f(start = 0);
  Real tlmDelay = TLM_Functions.TLMGetDelay(interfaceName);
initial algorithm
  assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
  TLM_Functions.TLMSetDebugMode(debugFlg);
equation
  v = der(flange_b.s);
  flange_b.f = f;
algorithm
  f := TLM_Functions.TLMGetForce1D(tlmPlugin, interfaceName, time, flange_b.s, v);
  annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, lineColor = {128, 0, 128}, fillColor = {128, 0, 128}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.90300000000001, 42.581}}, radius = 20), Line(visible = true, points = {{-63.226, -2.581}, {-28.387, -2.581}}, color = {128, 0, 128}, thickness = 5), Text(visible = true, origin = {33.696, 0}, textColor = {255, 255, 255}, extent = {{-53.696, -30}, {53.696, 30}}, textString = "1D Force")}));
end TLMForce1D;


    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {170, 9, 95}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {0, 2.861}, textColor = {255, 255, 255}, extent = {{-84.78700000000001, -62.861}, {84.78700000000001, 62.861}}, textString = "1D")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Interface_1D;


  package TLM_Interface_3D
    model TLMInterface3D
      TLM_Functions.TLMPlugin tlmPlugin = TLM_Functions.TLMPlugin();
      import F = Modelica.Mechanics.MultiBody.Frames;
      import M = Modelica.Mechanics.MultiBody.Frames.TransformationMatrices;
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      Real v[3];
      Real w[3];
      Real f[3](start = zeros(3));
      Real t[3](start = zeros(3));
      Real r[3];
      Real A[3, 3];
      Real AT[3, 3];
      Real tlmDelay = TLM_Functions.TLMGetDelay(interfaceName);
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation(Placement(visible = true, transformation(origin = {-67.9907, -3.03738}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-70, 0}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
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
//
// Transform force and moment into local system
      frame_a.f = M.resolve2(A, f);
      frame_a.t = M.resolve2(A, t);
    algorithm
      (f, t) := TLM_Functions.TLMGetForce(tlmPlugin, interfaceName, time, r, A, v, w);
      annotation(Diagram, Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = false, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(origin = {10, 0}, fillColor = {0, 170, 0}, fillPattern = FillPattern.Sphere, extent = {{-80, 70}, {70, -80}}), Text(origin = {15, -5}, extent = {{-65, 65}, {65, -65}}, textString = "TLM")}));
    end TLMInterface3D;
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {0, 170, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {-3.808, 3.341}, textColor = {255, 255, 255}, extent = {{-78.774, -56.659}, {78.774, 56.659}}, textString = "3D")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Interface_3D;

  package TLM_Interface_1D_to_3D
    model TLMInterface1Dto3D
      TLM_Functions.TLMPlugin tlmPlugin = TLM_Functions.TLMPlugin();
      import F = Modelica.Mechanics.MultiBody.Frames;
      import M = Modelica.Mechanics.MultiBody.Frames.TransformationMatrices;
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      output Real x;
      output Real v;
      input Real f;
      Real r[3];
      Real ft[3];
      Real Aa[3, 3];
      Real Ab[3, 3];
      //  Real AT[3, 3];
      Real tlmDelay = TLM_Functions.TLMGetDelay(interfaceName);
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation(Placement(visible = true, transformation(origin = {-67.9907, -3.03738}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-70, 0}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
      Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation(Placement(visible = true, transformation(origin = {-17.9907, 46.9626}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {82.009, -3.0374}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
      TLM_Functions.TLMSetDebugMode(debugFlg);
    equation
//
// World to frame transformation matrix
      Aa = frame_a.R.T;
      Ab = frame_b.R.T;
//
// Transform motion into world system
      r = frame_b.r_0 - frame_a.r_0;
      x = Modelica.Math.Vectors.length(r);
      v = der(x);
      ft = f * r / x;
//
// Transform force and moment into local system
      frame_a.f = M.resolve2(Aa, ft);
      frame_a.t = zeros(3);
      frame_b.f = M.resolve2(Ab, ft);
      frame_b.t = zeros(3);
    algorithm
      f := TLM_Functions.TLMGetForce1D(tlmPlugin, interfaceName, time, x, v);
      annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(origin = {10, 0}, fillColor = {0, 170, 0}, fillPattern = FillPattern.Sphere, extent = {{-80, 70}, {70, -80}}), Text(origin = {5, -5}, extent = {{-65, 65}, {65, -65}}, textString = "TLM", fontName = "MS Shell Dlg 2")}));
    end TLMInterface1Dto3D;
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {0, 170, 0}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {-3.808, 3.341}, textColor = {255, 255, 255}, extent = {{-78.774, -56.659}, {78.774, 56.659}}, textString = "3D")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Interface_1D_to_3D;

  package TLM_Sensors
    model TLMSensor
      TLMPlugin tlmPlugin = TLMPlugin();
      parameter String interfaceName = "tlm";
      parameter Boolean debugFlg = false;
      Real v[3] = {0, 0, 0};
      Real w[3] = {0, 0, 0};
      Real f[3](start = {0, 0, 0});
      Real t[3];
      Real r[3] = {0, 0, 0};
      Real A[3, 3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
      Real tlmDelay = TLMGetDelay(interfaceName);
    initial algorithm
      assert(tlmDelay > 0.0, "Bad TLM delay in" + interfaceName + ", give up");
      TLMSetDebugMode(debugFlg);
    algorithm
      (f, t) := TLMGetForce(tlmPlugin, interfaceName, time, r, A, v, w);
      annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, origin = {-10.651, -8.377000000000001}, lineColor = {0, 0, 255}, fillColor = {128, 0, 0}, fillPattern = FillPattern.Solid, extent = {{-89.349, -91.623}, {110.651, 108.377}}, radius = 20), Text(visible = true, origin = {-15.021, -3.686}, textColor = {255, 255, 255}, extent = {{-81.61199999999999, -54.575}, {111.654, 66.09099999999999}}, textString = "S")}), Diagram(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, lineColor = {0, 0, 255}, fillColor = {255, 255, 255}, fillPattern = FillPattern.Solid, extent = {{-74.476, -43.157}, {92.232, 51.048}}), Text(visible = true, origin = {-6.228, -0.744}, textColor = {0, 0, 255}, extent = {{-50.313, -33.645}, {68.833, 40.744}}, textString = "TLM Sensor")}));
    end TLMSensor;
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, origin = {-10.651, -8.377000000000001}, lineColor = {0, 0, 255}, fillColor = {128, 0, 0}, fillPattern = FillPattern.Solid, extent = {{-89.349, -91.623}, {110.651, 108.377}}, radius = 20), Text(visible = true, origin = {-15.021, -3.686}, textColor = {255, 255, 255}, extent = {{-81.61199999999999, -54.575}, {111.654, 66.09099999999999}}, textString = "S")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end TLM_Sensors;
  annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, fillColor = {0, 0, 255}, fillPattern = FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, textColor = {255, 255, 255}, extent = {{-86.59099999999999, -54.721}, {86.59099999999999, 54.721}}, textString = "TLM")}), uses(Modelica(version = "3.0")));
end TLM;
