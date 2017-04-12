model OutputInput
  TLM.TLM_Interface_Signal.TLMOutput tLMOutput1(debugFlg = false, interfaceName = "tlm2")  annotation(
    Placement(visible = true, transformation(origin = {24, 26}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_Signal.TLMInput tLMInput1(debugFlg = false, interfaceName = "tlm1")  annotation(
    Placement(visible = true, transformation(origin = {-58, 32}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Blocks.Sources.Sine sine1(freqHz = 0.5)  annotation(
    Placement(visible = true, transformation(origin = {-70, -4}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Math.Product product1 annotation(
    Placement(visible = true, transformation(origin = {-4, 22}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Continuous.Derivative derivative1 annotation(
    Placement(visible = true, transformation(origin = {-38, -4}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  TLM.TLM_Interface_Signal.TLMInput tLMInput2(interfaceName = "tlm3")  annotation(
    Placement(visible = true, transformation(origin = {-56, -34}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
equation
  connect(product1.u2, derivative1.y) annotation(
    Line(points = {{-16, 16}, {-20, 16}, {-20, -4}, {-26, -4}, {-26, -4}}, color = {0, 0, 127}));
  connect(sine1.y, derivative1.u) annotation(
    Line(points = {{-58, -4}, {-50, -4}}, color = {0, 0, 127}));
  connect(product1.y, tLMOutput1.u) annotation(
    Line(points = {{8, 22}, {16, 22}, {16, 26}, {16, 26}}, color = {0, 0, 127}));
  connect(tLMInput1.y, product1.u1) annotation(
    Line(points = {{-50, 32}, {-18, 32}, {-18, 28}, {-16, 28}}, color = {0, 0, 127}));
  annotation(
    uses(Modelica(version = "3.2.2")));
end OutputInput;