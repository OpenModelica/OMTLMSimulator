within ;
model HangingMass
  extends Modelica.Icons.Example;
  parameter Boolean animation = true "= true, if animation shall be enabled";
  inner Modelica.Mechanics.MultiBody.World world(g = 0) annotation(Placement(transformation(extent={{-80,60},
            {-60,80}},                                                                                                    rotation = 0)));
  Modelica.Mechanics.MultiBody.Forces.SpringDamperParallel springDamperParallel(       s_unstretched = 1, d = 5,
    c=1)                                                                                                     annotation(Placement(visible = true, transformation(origin={-0.32,50},    extent = {{-10,-10},{10,10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.BodyShape bodyShape(angles_fixed = true, animateSphere = true, animation = true, m = 16.5, r = {0,-0.5,0},
    r_0(                                                                                                    fixed =     true, start =     {0,-2,0}),r_CM = {0,0,0},
    v_0(                                                                                                    fixed =     true),w_0_fixed = true) annotation(Placement(visible = true, transformation(origin={1.016,
            -12},                                                                                                    extent = {{-10,-10},{10,10}}, rotation = -90)));
  Modelica.Mechanics.MultiBody.Parts.Body body(angles_fixed = false, w_0_fixed = false, m = 16.5, r_CM = {0,0.5,0}) annotation(Placement(visible = true, transformation(origin={1.283,
            -79.763},                                                                                                    extent = {{-10,-10},{10,10}}, rotation = -90)));
equation
  connect(bodyShape.frame_b,body.frame_a) annotation(Line(visible = true, origin = {-22.85,-56.316}, points={{23.866,
          34.316},{23.866,-4.434},{24.133,-4.434},{24.133,-13.447}}));
  connect(springDamperParallel.frame_b,bodyShape.frame_a) annotation(Line(visible = true, origin = {-23.652,-13.494}, points={{23.332,
          53.494},{23.332,-3.494},{24.668,-3.494},{24.668,11.494}}));
  connect(world.frame_b,springDamperParallel.frame_a) annotation(Line(visible = true, origin = {-36.214,26.667}, points={{-23.786,
          43.333},{35.894,43.333},{35.894,33.333}}));
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
    uses(Modelica(version="3.0")));
end HangingMass;

