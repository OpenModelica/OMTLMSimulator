within ;
package FMITLM


  package FMITLM_Interface_3D
  model FMITLMInterface3D
      import F = Modelica.Mechanics.MultiBody.Frames;
      import M = Modelica.Mechanics.MultiBody.Frames.TransformationMatrices;
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation(Placement(visible = true, transformation(origin = {-67.9907, -3.03738}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-67.9907, -3.03738}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
      parameter String interfaceName = "fmitlm";
      parameter Boolean debugFlg = false;
      output Real v[3];
      output Real w[3];
      input Real f[3](start = zeros(3));
      input Real t[3](start = zeros(3));
      output Real r[3];
      output Real A[3, 3];
      Real AT[3, 3];
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
      annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, extent = {{-100, -100}, {100, 100}}, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(visible = true, lineColor = {255, 255, 255}, fillColor = {0, 0, 255}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.90300000000001, 42.581}}, radius = 20), Line(visible = true, points = {{-63.226, -2.581}, {-28.387, -2.581}}, color = {0, 0, 255}, thickness = 5), Text(origin = {35.194, -1.225}, extent = {{-59.815, -21.225}, {59.815, 21.225}}, lineColor = {255, 255, 255}, fontSize = 48, textString = "3DFMITLM")}));
    end FMITLMInterface3D;

    model FMITLMInterface1Dto3D
      import F = Modelica.Mechanics.MultiBody.Frames;
      import M = Modelica.Mechanics.MultiBody.Frames.TransformationMatrices;
      Modelica.Mechanics.MultiBody.Interfaces.Frame_a frame_a annotation(Placement(visible = true, transformation(origin = {-67.9907, -3.03738}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {-97.9907, -3.03738}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
      parameter String interfaceName = "fmitlm";
      parameter Boolean debugFlg = false;
      output Real w[3];
      output Real phi[3];
      output Real r[3];
      input Real f;
      Real ft[3];
      Real Aa[3, 3];
      Real Ab[3, 3];
      Real x1,x2,x3;
      Real v[3];
      Real v1,v2,v3;
     // Real d[3];
  Modelica.Mechanics.MultiBody.Interfaces.Frame_b frame_b annotation(Placement(visible = true, transformation(origin = {-17.9907, 46.9626}, extent = {{-12, -12}, {12, 12}}, rotation = 0), iconTransformation(origin = {102.009, -3.0374}, extent = {{-12, -12}, {12, 12}}, rotation = 0)));
    equation
//
// World to frame transformation matrix
      Aa = frame_a.R.T;
      Ab = frame_b.R.T;

// Transform motion into world system
      r = frame_b.r_0-frame_a.r_0;
      v = der(r);
    
      x1 = Modelica.Math.Vectors.length(r);
      x2 =0.0;
      x3 = 0.0;
      
      v1=der(x1);
      v2=0.0;
      v3=0.0;
      
     
      w = zeros(3);
      phi = zeros(3);
      
      ft = f*r/x1;
     
      
// Transform force and moment into local system
      frame_a.f = M.resolve2(Aa, ft);
      frame_a.t = zeros(3);
      frame_b.f = M.resolve2(Ab, ft);
      frame_b.t = zeros(3);
      annotation(Diagram, Icon(coordinateSystem(preserveAspectRatio = false, initialScale = 0.1, grid = {10, 10}), graphics = {Rectangle(origin = {-30, 0}, lineColor = {255, 255, 255}, fillColor = {0, 0, 255}, fillPattern = FillPattern.HorizontalCylinder, extent = {{-29.548, -45.806}, {96.903, 42.581}}, radius = 20), Line(points = {{-93.226, -2.581}, {101.613, -2.581}}, color = {0, 0, 255}, thickness = 5), Text(origin = {5.194, -1.225}, lineColor = {255, 255, 255}, extent = {{-59.815, -21.225}, {59.815, 21.225}}, textString = "3DFMITLM", fontSize = 48, fontName = "MS Shell Dlg 2")}), uses(Modelica(version = "3.2.2")));
    end FMITLMInterface1Dto3D;
    annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics={  Rectangle(visible = true, fillColor = {0, 170, 0},
              fillPattern =                                                                                                    FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, origin = {-3.808, 3.341}, textColor = {255, 255, 255}, extent = {{-78.774, -56.659}, {78.774, 56.659}}, textString = "3D")}), Diagram(coordinateSystem(extent = {{-148.5, -105}, {148.5, 105}}, preserveAspectRatio = true, initialScale = 0.1, grid = {5, 5})));
  end FMITLM_Interface_3D;
  annotation(Icon(coordinateSystem(extent = {{-100, -100}, {100, 100}}, preserveAspectRatio = true, initialScale = 0.1, grid = {10, 10}), graphics={  Rectangle(visible = true, fillColor = {0, 0, 255},
            fillPattern =                                                                                                    FillPattern.Solid, extent = {{-100, -100}, {100, 100}}, radius = 20), Text(visible = true, textColor = {255, 255, 255}, extent = {{-86.59099999999999, -54.721}, {86.59099999999999, 54.721}}, textString = "TLM")}), uses(Modelica(
          version="3.2.1")));
end FMITLM;
