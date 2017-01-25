model Pendulum

  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder(
    diameter=0.2,
    innerDiameter=0,
    r_shape={0,0,0},
    r={0.5,0,0},
    length=0.5)
    annotation (Placement(transformation(extent={{-10,-10},{10,10}})));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed annotation (Placement(
        transformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={-52,20})));
  inner Modelica.Mechanics.MultiBody.World world(
    enableAnimation=true,
    animateWorld=true,
    animateGravity=true)
    annotation (Placement(transformation(extent={{30,56},{50,76}})));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute
    annotation (Placement(transformation(extent={{-44,-10},{-24,10}})));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(
    diameter=0.2,
    innerDiameter=0,
    r={0.5,0,0},
    length=0.5,
    r_shape={0,0,0})
    annotation (Placement(transformation(extent={{-4,-62},{16,-42}})));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed1(r={1,0,0})
                                                 annotation (Placement(
        transformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={-44,-34})));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1
    annotation (Placement(transformation(extent={{-38,-62},{-18,-42}})));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder2(
    diameter=0.2,
    innerDiameter=0,
    r={0.5,0,0},
    length=0.5,
    r_shape={0,0,0},
    r_0(start={1.5,0,0}))
    annotation (Placement(transformation(extent={{64,-62},{84,-42}})));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder3(
    diameter=0.2,
    innerDiameter=0,
    r_shape={0,0,0},
    r={0.5,0,0},
    length=0.5)
    annotation (Placement(transformation(extent={{24,-10},{44,10}})));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D(interfaceName="tlm1")
    annotation (Placement(transformation(extent={{16,-62},{36,-42}})));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(interfaceName="tlm2")
    annotation (Placement(transformation(
        extent={{-10,-10},{10,10}},
        rotation=180,
        origin={54,-52})));
equation
  connect(revolute.frame_b, bodyCylinder.frame_a) annotation (Line(
      points={{-24,0},{-18,0},{-10,0}},
      color={95,95,95},
      thickness=0.5));
  connect(fixed.frame_b, revolute.frame_a) annotation (Line(
      points={{-52,10},{-50,10},{-50,0},{-44,0}},
      color={95,95,95},
      thickness=0.5));
  connect(revolute1.frame_b, bodyCylinder1.frame_a) annotation (Line(
      points={{-18,-52},{-12,-52},{-4,-52}},
      color={95,95,95},
      thickness=0.5));
  connect(fixed1.frame_b, revolute1.frame_a) annotation (Line(
      points={{-44,-44},{-44,-44},{-44,-52},{-38,-52}},
      color={95,95,95},
      thickness=0.5));
  connect(bodyCylinder3.frame_a, bodyCylinder.frame_b) annotation (Line(
      points={{24,0},{18,0},{10,0}},
      color={95,95,95},
      thickness=0.5));
  connect(tLMInterface3D.frame_a, bodyCylinder1.frame_b) annotation (Line(
      points={{19.2009,-52.3037},{18,-52.3037},{18,-52},{16,-52}},
      color={95,95,95},
      thickness=0.5));
  connect(tLMInterface3D1.frame_a, bodyCylinder2.frame_a) annotation (Line(
      points={{60.7991,-51.6963},{62,-51.6963},{62,-52},{64,-52}},
      color={95,95,95},
      thickness=0.5));
  annotation (
    Icon(coordinateSystem(preserveAspectRatio=false)),
    Diagram(coordinateSystem(preserveAspectRatio=false)),
    uses(Modelica(version="3.2.1")));
end Pendulum;
