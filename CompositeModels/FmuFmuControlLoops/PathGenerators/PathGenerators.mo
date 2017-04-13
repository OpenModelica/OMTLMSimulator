model PathGenerators
  Modelica.Blocks.Sources.Sine sine1(freqHz = 0.4) annotation(
    Placement(visible = true, transformation(origin = {-44, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  output Real phi_ref;
equation
  phi_ref = sine1.y;
  annotation(
    uses(Modelica(version = "3.2.2")));
end PathGenerators;