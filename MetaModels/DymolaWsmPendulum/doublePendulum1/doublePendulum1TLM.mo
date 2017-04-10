within ;
import "C:/OpenModelica/ModelicaModels/TLM.mo";

model doublePendulum1TLM
  extends Modelica.Icons.Example;
  parameter Boolean animation = true "= true, if animation shall be enabled";
  TLM.TLM_Interface_3D.TLMInterface3D TLMInterface 
                                        annotation (Placement(transformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={82,16})));
  annotation(experiment(StopTime = 10), Documentation(info = "<html>
<p>
This example demonstrates:
</p>
<ul>
<li>The animation of spring and damper components</li>
<li>A body can be freely moving without any connection to a joint.
    In this case body coordinates are used automatically as
    states (whenever joints are present, it is first tried to
    use the generalized coordinates of the joints as states).</li>
<li>If a body is freely moving, the initial position and velocity of the body
    can be defined with the \"Initialization\" menu as shown with the
    body \"body1\" in the left part (click on \"Initialization\").</li>
</ul>

<IMG src=\"modelica://Modelica/Resources/Images/Mechanics/MultiBody/Examples/Elementary/SpringDamperSystem.png\"
ALT=\"model Examples.Elementary.SpringDamperSystem\">

</html>"),
    uses(Modelica(version="3.0")),
    Diagram(coordinateSystem(preserveAspectRatio=true,  extent={{-100,-100},{
            100,100}}), graphics));
  inner Modelica.Mechanics.MultiBody.World world annotation (Placement(
        transformation(extent={{-78,10},{-58,30}},rotation=0)));
  Modelica.Mechanics.MultiBody.Joints.Revolute revolute1(useAxisFlange=true,phi(fixed=true),
      w(fixed=true))                                             annotation (Placement(transformation(extent={{-38,10},
            {-18,30}}, rotation=0)));
  Modelica.Mechanics.Rotational.Components.Damper damper(
                                              d=0.1) 
    annotation (Placement(transformation(extent={{-38,50},{-18,70}}, rotation=0)));
  Modelica.Mechanics.MultiBody.Parts.BodyBox boxBody1(             width=0.06,
    r={0.25,0,0},
    r_0(fixed=true, start={0,0,0}),
    v_0(fixed=true, start={0,0,0})) 
    annotation (Placement(transformation(extent={{-6,10},{14,30}}, rotation=0)));
equation
  connect(damper.flange_b,revolute1. axis) annotation (Line(points={{-18,60},{
          -14,60},{-14,38},{-28,38},{-28,30}}, color={0,0,0}));
  connect(revolute1.support,damper. flange_a) annotation (Line(points={{-34,30},
          {-34,38},{-48,38},{-48,60},{-38,60}}, color={0,0,0}));
  connect(revolute1.frame_b,boxBody1. frame_a) 
    annotation (Line(
      points={{-18,20},{-6,20}},
      color={95,95,95},
      thickness=0.5));
  connect(world.frame_b,revolute1. frame_a) 
    annotation (Line(
      points={{-58,20},{-38,20}},
      color={95,95,95},
      thickness=0.5));
  connect(TLMInterface.frame_a, boxBody1.frame_b) annotation (Line(
      points={{81.6963,22.7991},{14,22.7991},{14,20}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
end doublePendulum1TLM;
