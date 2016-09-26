within ;
model Shaft
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder(r={1,0,0})
    annotation (Placement(transformation(extent={{22,-4},{42,16}})));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder1(r={1,0,0})
    annotation (Placement(transformation(extent={{-28,-4},{-8,16}})));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute
    annotation (Placement(transformation(extent={{-56,-4},{-36,16}})));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed
    annotation (Placement(transformation(extent={{-88,-4},{-68,16}})));
  Modelica.Blocks.Sources.Ramp ramp(
    height=1000,
    duration=5,
    offset=0,
    startTime=0)
    annotation (Placement(transformation(extent={{48,24},{68,44}})));
  Modelica.Blocks.Sources.Constant const(k=1000)
    annotation (Placement(transformation(extent={{34,52},{54,72}})));
  Modelica.Blocks.Sources.Constant const1(k=500)
    annotation (Placement(transformation(extent={{30,76},{50,96}})));
  inner Modelica.Mechanics.MultiBody.World world
    annotation (Placement(transformation(extent={{-76,56},{-56,76}})));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder2(r={1,0,0}, r_0(
        start={1,0,0}))
    annotation (Placement(transformation(extent={{36,-68},{56,-48}})));
  Modelica.Mechanics.MultiBody.Parts.BodyCylinder bodyCylinder3(r={1,0,0})
    annotation (Placement(transformation(extent={{-36,-68},{-16,-48}})));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1
    annotation (Placement(transformation(extent={{-64,-68},{-44,-48}})));
  Modelica.Mechanics.MultiBody.Parts.Fixed fixed1
    annotation (Placement(transformation(extent={{-96,-68},{-76,-48}})));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D(interfaceName="tlm1")
    annotation (Placement(transformation(extent={{-14,-68},{6,-48}})));
  TLM.TLM_Interface_3D.TLMInterface3D tLMInterface3D1(interfaceName="tlm2")
    annotation (Placement(transformation(
        extent={{-10,-10},{10,10}},
        rotation=180,
        origin={26,-58})));
  Modelica.Mechanics.MultiBody.Forces.WorldForceAndTorque forceAndTorque
    annotation (Placement(transformation(
        extent={{-10,-10},{10,10}},
        rotation=180,
        origin={66,6})));
  Modelica.Blocks.Sources.Constant const2(k=200)
    annotation (Placement(transformation(extent={{2,28},{22,48}})));
  Modelica.Blocks.Sources.Constant const3(k=100)
    annotation (Placement(transformation(extent={{2,56},{22,76}})));
  Modelica.Blocks.Sources.Constant const4(k=1000)
    annotation (Placement(transformation(extent={{-4,84},{16,104}})));
  Modelica.Mechanics.MultiBody.Forces.WorldForceAndTorque forceAndTorque1
    annotation (Placement(transformation(
        extent={{-10,-10},{10,10}},
        rotation=180,
        origin={72,-58})));
equation
  connect(revolute.frame_b, bodyCylinder1.frame_a) annotation (Line(
      points={{-36,6},{-32,6},{-28,6}},
      color={95,95,95},
      thickness=0.5));
  connect(fixed.frame_b, revolute.frame_a) annotation (Line(
      points={{-68,6},{-62,6},{-56,6}},
      color={95,95,95},
      thickness=0.5));
  connect(bodyCylinder.frame_a, bodyCylinder1.frame_b) annotation (Line(
      points={{22,6},{7,6},{-8,6}},
      color={95,95,95},
      thickness=0.5));
  connect(revolute1.frame_b, bodyCylinder3.frame_a) annotation (Line(
      points={{-44,-58},{-40,-58},{-36,-58}},
      color={95,95,95},
      thickness=0.5));
  connect(fixed1.frame_b, revolute1.frame_a) annotation (Line(
      points={{-76,-58},{-70,-58},{-64,-58}},
      color={95,95,95},
      thickness=0.5));
  connect(tLMInterface3D.frame_a, bodyCylinder3.frame_b) annotation (Line(
      points={{-10.7991,-58.3037},{-10,-58.3037},{-10,-58},{-12,-58},{-16,-58}},
      color={95,95,95},
      thickness=0.5));

  connect(tLMInterface3D1.frame_a, bodyCylinder2.frame_a) annotation (Line(
      points={{32.7991,-57.6963},{33.3995,-57.6963},{33.3995,-58},{36,-58}},
      color={95,95,95},
      thickness=0.5));
  connect(forceAndTorque.frame_b, bodyCylinder.frame_b) annotation (Line(
      points={{56,6},{42,6}},
      color={95,95,95},
      thickness=0.5));
  connect(const2.y, forceAndTorque.force[1]) annotation (Line(points={{23,38},{
          56,38},{88,38},{88,13.3333},{78,13.3333}}, color={0,0,127}));
  connect(const3.y, forceAndTorque.force[2]) annotation (Line(points={{23,66},{
          56,66},{90,66},{90,12},{78,12}}, color={0,0,127}));
  connect(const4.y, forceAndTorque.force[3]) annotation (Line(points={{17,94},{
          92,94},{92,10.6667},{78,10.6667}}, color={0,0,127}));
  connect(forceAndTorque1.frame_b, bodyCylinder2.frame_b) annotation (Line(
      points={{62,-58},{62,-58},{56,-58}},
      color={95,95,95},
      thickness=0.5));
  connect(const.y, forceAndTorque.torque[2]) annotation (Line(points={{55,62},{
          96,62},{96,0},{78,0}},         color={0,0,127}));
  connect(const.y, forceAndTorque1.torque[2]) annotation (Line(points={{55,62},
          {55,62},{96,62},{96,-64},{84,-64}}, color={0,0,127}));
  connect(const4.y, forceAndTorque1.force[3]) annotation (Line(points={{17,94},
          {54,94},{92,94},{92,-53.3333},{84,-53.3333}}, color={0,0,127}));
  connect(const3.y, forceAndTorque1.force[2]) annotation (Line(points={{23,66},
          {56,66},{90,66},{90,-52},{84,-52}}, color={0,0,127}));
  connect(const2.y, forceAndTorque1.force[1]) annotation (Line(points={{23,38},
          {56,38},{88,38},{88,-50.6667},{84,-50.6667}}, color={0,0,127}));
  connect(ramp.y, forceAndTorque.torque[3]) annotation (Line(points={{69,34},{
          84,34},{98,34},{98,-1.33333},{78,-1.33333}}, color={0,0,127}));
  connect(ramp.y, forceAndTorque1.torque[3]) annotation (Line(points={{69,34},{
          84,34},{98,34},{98,-65.3333},{84,-65.3333}}, color={0,0,127}));
  connect(const1.y, forceAndTorque.torque[1]) annotation (Line(points={{51,86},
          {74,86},{94,86},{94,1.33333},{78,1.33333}}, color={0,0,127}));
  connect(const1.y, forceAndTorque1.torque[1]) annotation (Line(points={{51,86},
          {72,86},{94,86},{94,-62.6667},{84,-62.6667}}, color={0,0,127}));
  annotation (
    Icon(coordinateSystem(preserveAspectRatio=false)),
    Diagram(coordinateSystem(preserveAspectRatio=false)),
    uses(Modelica(version="3.2.1")));
end Shaft;
