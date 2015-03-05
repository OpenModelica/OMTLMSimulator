!
!-------------------------- Default Units for Model ---------------------------!
!
!
defaults units  &
   length = mm  &
   angle = deg  &
   force = newton  &
   mass = kg  &
   time = sec
!
defaults units  &
   coordinate_system_type = cartesian  &
   orientation_type = body313
!
!------------------------ Default Attributes for Model ------------------------!
!
!
defaults attributes  &
   inheritance = bottom_up  &
   icon_visibility = on  &
   grid_visibility = off  &
   size_of_icons = 50.0  &
   spacing_for_grid = 1000.0
!
!------------------------------ Adams/View Model ------------------------------!
!
!
model create  &
   model_name = Spindle
!
view erase
!
!--------------------------------- Materials ----------------------------------!
!
!
material create  &
   material_name = .Spindle.steel  &
   adams_id = 1  &
   youngs_modulus = 2.07E+005  &
   poissons_ratio = 0.29  &
   density = 7.801E-006
!
!-------------------------------- Rigid Parts ---------------------------------!
!
! Create parts and their dependent markers and graphics
!
!----------------------------------- ground -----------------------------------!
!
!
! ****** Ground Part ******
!
defaults model  &
   part_name = ground
!
defaults coordinate_system  &
   default_coordinate_system = .Spindle.ground
!
! ****** Markers for current part ******
!
marker create  &
   marker_name = .Spindle.ground.MARKER_2  &
   adams_id = 2  &
   location = -500.0, 0.0, 0.0  &
   orientation = 0.0d, 0.0d, 0.0d
!
marker create  &
   marker_name = .Spindle.ground.MARKER_6  &
   adams_id = 6  &
   location = 0.0, 0.0, 0.0  &
   orientation = 90.0d, 90.0d, 0.0d
!
marker create  &
   marker_name = .Spindle.ground.MARKER_9  &
   adams_id = 9  &
   location = -200.0, 0.0, 0.0  &
   orientation = 90.0d, 90.0d, 0.0d
!
! ****** Floating Markers for current part ******
!
floating_marker create  &
   floating_marker_name = .Spindle.ground.FMARKER_7  &
   adams_id = 7
!
part create rigid_body mass_properties  &
   part_name = .Spindle.ground  &
   material_type = .Spindle.steel
!
part attributes  &
   part_name = .Spindle.ground  &
   name_visibility = off
!
!----------------------------------- PART_2 -----------------------------------!
!
!
defaults coordinate_system  &
   default_coordinate_system = .Spindle.ground
!
part create rigid_body name_and_position  &
   part_name = .Spindle.PART_2  &
   adams_id = 2  &
   location = 0.0, 0.0, 0.0  &
   orientation = 0.0d, 0.0d, 0.0d
!
defaults coordinate_system  &
   default_coordinate_system = .Spindle.PART_2
!
! ****** Markers for current part ******
!
marker create  &
   marker_name = .Spindle.PART_2.MARKER_1  &
   adams_id = 1  &
   location = 0.0, 0.0, 0.0  &
   orientation = 270.0d, 90.0d, 180.0d
!
marker create  &
   marker_name = .Spindle.PART_2.cm  &
   adams_id = 4  &
   location = -200.0, 0.0, 0.0  &
   orientation = 90.0d, 90.0d, 0.0d
!
marker create  &
   marker_name = .Spindle.PART_2.MARKER_3  &
   adams_id = 3  &
   location = -400.0, 0.0, 0.0  &
   orientation = 0.0d, 0.0d, 0.0d
!
marker create  &
   marker_name = .Spindle.PART_2.MARKER_5  &
   adams_id = 5  &
   location = 0.0, 0.0, 0.0  &
   orientation = 90.0d, 90.0d, 0.0d
!
marker create  &
   marker_name = .Spindle.PART_2.MARKER_8  &
   adams_id = 8  &
   location = -200.0, 0.0, 0.0  &
   orientation = 90.0d, 90.0d, 0.0d
!
part create rigid_body mass_properties  &
   part_name = .Spindle.PART_2  &
   material_type = .Spindle.steel
!
! ****** Graphics for current part ******
!
geometry create shape cylinder  &
   cylinder_name = .Spindle.PART_2.CYLINDER_1  &
   adams_id = 1  &
   center_marker = .Spindle.PART_2.MARKER_1  &
   angle_extent = 360.0  &
   length = 400.0  &
   radius = 50.0  &
   side_count_for_body = 20  &
   segment_count_for_ends = 20
!
part attributes  &
   part_name = .Spindle.PART_2  &
   color = RED  &
   name_visibility = off
!
!----------------------------------- Joints -----------------------------------!
!
!
constraint create joint revolute  &
   joint_name = .Spindle.JOINT_1  &
   adams_id = 1  &
   i_marker_name = .Spindle.PART_2.MARKER_8  &
   j_marker_name = .Spindle.ground.MARKER_9
!
constraint attributes  &
   constraint_name = .Spindle.JOINT_1  &
   name_visibility = off
!
!----------------------------------- Forces -----------------------------------!
!
!
force create direct general_force  &
   general_force_name = .Spindle.GFORCE_1  &
   adams_id = 1  &
   i_marker_name = .Spindle.PART_2.MARKER_5  &
   j_floating_marker_name = .Spindle.ground.FMARKER_7  &
   ref_marker_name = .Spindle.ground.MARKER_6  &
   user_function = 5.0, 6.0, 6.0, 1.0  &
   routine = "tlmadams::"
!
!---------------------------------- Motions -----------------------------------!
!
!
constraint create motion_generator  &
   motion_name = .Spindle.MOTION_1  &
   adams_id = 1  &
   type_of_freedom = rotational  &
   joint_name = .Spindle.JOINT_1  &
   function = ""
!
constraint attributes  &
   constraint_name = .Spindle.MOTION_1  &
   name_visibility = off
!
!-------------------------- Adams/View UDE Instances --------------------------!
!
!
defaults coordinate_system  &
   default_coordinate_system = .Spindle.ground
!
undo begin_block suppress = yes
!
ude create instance  &
   instance_name = .Spindle.SPRING_1  &
   definition_name = .MDI.Forces.spring  &
   location = 0.0, 0.0, 0.0  &
   orientation = 0.0, 0.0, 0.0
!
ude attributes  &
   instance_name = .Spindle.SPRING_1  &
   color = RED
!
!-------------------------- Adams/View UDE Instance ---------------------------!
!
!
variable modify  &
   variable_name = .Spindle.SPRING_1.i_marker  &
   object_value = (.Spindle.ground.MARKER_2)
!
variable modify  &
   variable_name = .Spindle.SPRING_1.j_marker  &
   object_value = (.Spindle.PART_2.MARKER_3)
!
variable modify  &
   variable_name = .Spindle.SPRING_1.stiffness_mode  &
   string_value = "linear"
!
variable modify  &
   variable_name = .Spindle.SPRING_1.stiffness_coefficient  &
   real_value = (10(newton/mm))
!
variable modify  &
   variable_name = .Spindle.SPRING_1.stiffness_spline  &
   object_value = (NONE)
!
variable modify  &
   variable_name = .Spindle.SPRING_1.damping_mode  &
   string_value = "linear"
!
variable modify  &
   variable_name = .Spindle.SPRING_1.damping_coefficient  &
   real_value = (2.5E-002(newton-sec/mm))
!
variable modify  &
   variable_name = .Spindle.SPRING_1.damping_spline  &
   object_value = (NONE)
!
variable modify  &
   variable_name = .Spindle.SPRING_1.free_length_mode  &
   string_value = "Design_Length"
!
variable modify  &
   variable_name = .Spindle.SPRING_1.free_length  &
   real_value = 1.0
!
variable modify  &
   variable_name = .Spindle.SPRING_1.preload  &
   real_value = 0.0
!
variable modify  &
   variable_name = .Spindle.SPRING_1.i_dynamic_visibility  &
   string_value = "On"
!
variable modify  &
   variable_name = .Spindle.SPRING_1.j_dynamic_visibility  &
   string_value = "Off"
!
variable modify  &
   variable_name = .Spindle.SPRING_1.spring_visibility  &
   string_value = "depends"
!
variable modify  &
   variable_name = .Spindle.SPRING_1.damper_visibility  &
   string_value = "depends"
!
ude modify instance  &
   instance_name = .Spindle.SPRING_1
!
undo end_block
!
!------------------------------ Dynamic Graphics ------------------------------!
!
!
defaults coordinate_system  &
   default_coordinate_system = .Spindle.ground
!
geometry create shape force  &
   force_name = .Spindle.GFORCE_1_force_graphic_1  &
   force_element_name = .Spindle.GFORCE_1  &
   applied_at_marker_name = .Spindle.PART_2.MARKER_5
!
!---------------------------------- Accgrav -----------------------------------!
!
!
force create body gravitational  &
   gravity_field_name = gravity  &
   x_component_gravity = 0.0  &
   y_component_gravity = -9806.65  &
   z_component_gravity = 0.0
!
!----------------------------- Analysis settings ------------------------------!
!
!
!----------------------------- Simulation Scripts -----------------------------!
!
!
simulation script create  &
   sim_script_name = .Spindle.Last_Sim  &
   commands =   &
              "simulation single_run transient type=auto_select end_time=2.0 number_of_steps=50 model_name=.Spindle initial_static=no"
!
!---------------------------- Function definitions ----------------------------!
!
!
constraint modify motion_generator  &
   motion_name = .Spindle.MOTION_1  &
   function = "90.0d *sin(100* time)"
!
!-------------------------- Adams/View UDE Instance ---------------------------!
!
!
ude modify instance  &
   instance_name = .Spindle.SPRING_1
!
!--------------------------- Expression definitions ---------------------------!
!
!
defaults coordinate_system  &
   default_coordinate_system = ground
!
material modify  &
   material_name = .Spindle.steel  &
   youngs_modulus = (2.07E+011(Newton/meter**2))  &
   density = (7801.0(kg/meter**3))
!
geometry modify shape cylinder  &
   cylinder_name = .Spindle.PART_2.CYLINDER_1  &
   length = (400.0mm)  &
   radius = (50.0mm)
!
geometry modify shape force  &
   force_name = .Spindle.GFORCE_1_force_graphic_1  &
   applied_at_marker_name = (.Spindle.GFORCE_1.i)
!
model display  &
   model_name = Spindle
