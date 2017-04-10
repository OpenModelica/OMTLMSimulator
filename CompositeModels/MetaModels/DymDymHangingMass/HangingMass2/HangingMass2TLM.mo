within ;


model HangingMass2TLM
  extends Modelica.Icons.Example;
  parameter Boolean animation = true "= true, if animation shall be enabled";
  inner Modelica.Mechanics.MultiBody.World world(g = 0, enableAnimation=false) 
                                                        annotation(Placement(transformation(extent={{38,58},
            {58,78}},                                                                                                    rotation = 0)));
  Modelica.Mechanics.MultiBody.Parts.Body body(                                         m = 16.5, r_CM = {0,0.5,0},
    v_0(fixed=true),
    r_0(fixed=true, start={0,-2.5,0}),
    angles_fixed=true,
    w_0_fixed=true)                                                                                                 annotation(Placement(visible = true, transformation(origin={-22.717,
            -13.763},                                                                                                    extent = {{-10,-10},{10,10}}, rotation = -90)));
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
    Diagram(coordinateSystem(preserveAspectRatio=true, extent={{-100,-100},{100,
            100}}), graphics),
    uses(Modelica(version="3.0")));
  TLM.TLM_Interface_3D.TLMInterface3D TLMInterface 
    annotation (Placement(transformation(extent={{-4,-2},{16,18}})));
equation
  connect(TLMInterface.frame_a, body.frame_a) annotation (Line(
      points={{-0.79907,7.69626},{-11.3995,7.69626},{-11.3995,-3.763},{-22.717,
          -3.763}},
      color={95,95,95},
      thickness=0.5,
      smooth=Smooth.None));
end HangingMass2TLM;

