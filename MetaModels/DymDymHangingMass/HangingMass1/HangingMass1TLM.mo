within ;


model HangingMass1TLM
  extends Modelica.Icons.Example;
  parameter Boolean animation = true "= true, if animation shall be enabled";
  inner Modelica.Mechanics.MultiBody.World world(g = 0, enableAnimation=false) 
                                                        annotation(Placement(transformation(extent = {{-80,20},{-60,40}}, rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.SpringDamperParallel springDamperParallel(       s_unstretched = 1, d = 5,
    c=1)                                                                                                     annotation(Placement(visible = true, transformation(origin={-22.32,
            10},                                                                                                    extent = {{-10,-10},{10,10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(angles_fixed = true, animateSphere = true, animation = true, m = 16.5, r = {0,-0.5,0},
    r_0(                                                                                                    fixed =     true, start =     {0,-2,0}),r_CM = {0,0,0},
    v_0(                                                                                                    fixed =     true),w_0_fixed = true) annotation(Placement(visible = true, transformation(origin={-22.984,
            -34},                                                                                                    extent = {{-10,-10},{10,10}}, rotation = -90)));
  TLM.TLM_Interface_3D.TLMInterface3D TLMInterface annotation (Placement(transformation(
        extent={{-10,-10},{10,10}},
        rotation=270,
        origin={-22,-66})));
equation
  connect(springDamperParallel.frame_b,bodyShape.frame_a) annotation(Line(visible = true, origin = {-23.652,-13.494}, points={{1.332,
          13.494},{1.332,-3.494},{0.668,-3.494},{0.668,-10.506}}));
  connect(world.frame_b,springDamperParallel.frame_a) annotation(Line(visible = true, origin = {-36.214,26.667}, points={{-23.786,
          3.333},{13.894,3.333},{13.894,-6.667}}));
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
    Diagram(coordinateSystem(preserveAspectRatio=false, extent={{-100,-100},{
            100,100}}), graphics));
  connect(bodyShape.frame_b, TLMInterface.frame_a) annotation (Line(
      points={{-22.984,-44},{-22.3037,-44},{-22.3037,-59.2009}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
end HangingMass1TLM;
