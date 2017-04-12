model InputOutput
  TLM.TLM_Interface_Signal.TLMOutput tLMOutput1(debugFlg = false, interfaceName = "tlm1")  annotation(
    Placement(visible = true, transformation(origin = {24, 30}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_Signal.TLMInput tLMInput1(debugFlg = false, interfaceName = "tlm2")  annotation(
    Placement(visible = true, transformation(origin = {-52, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Blocks.Continuous.Derivative derivative1 annotation(
    Placement(visible = true, transformation(origin = {-8, 30}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_Signal.TLMOutput tLMOutput2(interfaceName = "tlm3")  annotation(
    Placement(visible = true, transformation(origin = {20, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Math.Gain gain1(k = 5)  annotation(
    Placement(visible = true, transformation(origin = {-18, -20}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Sources.Trapezoid trapezoid1(falling = 0.2, period = 1, rising = 0.2, width = 0.2)  annotation(
    Placement(visible = true, transformation(origin = {-44, 30}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(trapezoid1.y, derivative1.u) annotation(
    Line(points = {{-33, 30}, {-20, 30}}, color = {0, 0, 127}));
  connect(gain1.y, tLMOutput2.u) annotation(
    Line(points = {{-6, -20}, {12, -20}}, color = {0, 0, 127}));
  connect(tLMInput1.y, gain1.u) annotation(
    Line(points = {{-44, -20}, {-30, -20}, {-30, -20}, {-30, -20}, {-30, -20}}, color = {0, 0, 127}));
  connect(derivative1.y, tLMOutput1.u) annotation(
    Line(points = {{4, 30}, {16, 30}, {16, 30}, {16, 30}}, color = {0, 0, 127}));
  annotation(
    uses(Modelica(version = "3.2.2")));
end InputOutput;