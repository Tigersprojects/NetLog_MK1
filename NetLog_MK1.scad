/*
  Case for the NetLog MK1.

  Use with M3 threaded inserts
  and M3 hex socket cap screws.
  
  This file is a mess.
*/


$fn=30;

// Area for the components, e.g. GPS module, MCU, antenna //
case_x = 60;
case_y = 70;
case_z = 9;

// Mainboard size //
mainb_l = 65.5;
mainb_w = 25.5;
mainb_h = 1;

// GPS module size //
gps_l = 36.75;
gps_w = 26.25;
gps_h = 1.5;

// GPS antenna size //
gpsa_l = 28.5;
gpsa_w = 28.5;
gpsa_h = 9;

// Mainboard USB-C connector offsets //
mainb_usb_x = 16;
mainb_usb_z = 0.75;

// Tolerance offset //
tolerance_offset = 0.25;

// Other stuff //
wall_thickness = 2;

box = true;
lid = true;

lid_thickness = 4;

text_font = "Space Mono:style=Bold";


// A hole for a screw //
module screwHole() {
  translate([2.75, 2.875, 3]) cylinder(1, d=3.5);
  translate([2.75, 2.875]) cylinder(3, d=5.75);
}


// Box //
if (box) {

  // Rounded corners //
  for (i = [0 : 1 : 1]) {
    difference() {

      translate([
        5 + i * (case_x + wall_thickness * 2), 5
      ]) hull() {
        
        cylinder(case_z + wall_thickness, d=5 * 2);
        
        translate([0, case_y - 10 + wall_thickness * 2])
          cylinder(case_z + wall_thickness, d=5 * 2);
      }

      translate([
        5 + i * (case_x + wall_thickness * 2 - 5), 0
      ]) cube([
        5, case_y + wall_thickness * 2,
        case_z + wall_thickness
      ]);
      

      // Holes for screws //
      for (j = [0 : 1 : 1]) translate([
        i * (case_x + 5 + wall_thickness * 2 - 2) + 3.5,

        10 + j *
        (case_y - 20 + wall_thickness * 2),
        
        case_z + wall_thickness - 10.5
      ]) cylinder(10.5, d=4);
    }
  }

  translate([5, 0]) {

    // Box for the components //
    difference() {
      cube([
        case_x + wall_thickness * 2,
        case_y + wall_thickness * 2,
        case_z + wall_thickness
      ]);
      
      translate([wall_thickness, wall_thickness, wall_thickness]) cube([
        case_x,
        case_y,
        case_z
      ]);
      

      // Holes for screws //
      for (i = [0 : 1 : 1]) for (j = [0 : 1 : 1]) translate([
        i * (case_x + 5 + wall_thickness * 2 - 2) - 1.5,
        
        10 + j *
        (case_y - 20 + wall_thickness * 2),
        
        case_z + wall_thickness - 10.5
      ]) cylinder(10.5, d=4);


      // Small pits under GPS module //
      translate([
        case_x - gps_w + wall_thickness,
        case_y - gps_l + wall_thickness,
        wall_thickness / 2
      ]) cube([gps_w, 5, wall_thickness / 2]);

      translate([
        case_x - gps_w + wall_thickness,
        case_y - 5 + wall_thickness,
        wall_thickness / 2
      ]) cube([gps_w, 5, wall_thickness / 2]);


      // Hole for USB-C port //
      translate([
        wall_thickness + 1.875 + mainb_usb_x,
        wall_thickness * 2 + case_y,
        wall_thickness + 1.875 + mainb_usb_z
      ]) { 

        rotate([90, 90]) {
          hull() {
            cylinder(wall_thickness, d=3.5);
            translate([0, 5]) cylinder(wall_thickness, d=3.5);
          }
        }
      }

      // Make the wall thinner where the mainboard goes
      translate([wall_thickness, wall_thickness + case_y, wall_thickness])
        cube([mainb_w, wall_thickness / 2, case_z]);
    }

    // Holders for the mainboard //
    translate([wall_thickness + mainb_w, wall_thickness, wall_thickness])
      cube([wall_thickness, case_y, mainb_h]);
  
    translate([wall_thickness, wall_thickness, wall_thickness])
      cube([mainb_w, case_y - mainb_l + wall_thickness / 2, mainb_h]); 

    // Holders for the GPS module //
    translate([case_x - gps_w, case_y - gps_l, wall_thickness])
      cube([wall_thickness, gps_l + wall_thickness, gps_h]);
  
    translate([
      case_x - gps_w + wall_thickness,
      case_y - gps_l,
      wall_thickness
    ]) cube([gps_w, wall_thickness, gps_h]);
  

    // Holders for the GPS antenna //
    difference() {
      translate([case_x - gpsa_w, wall_thickness, wall_thickness])
        cube([gpsa_w + wall_thickness, gpsa_l + wall_thickness, gpsa_h]);

      translate([
        case_x - gpsa_w + wall_thickness,
        wall_thickness,
        wall_thickness
      ]) cube([gpsa_w, gpsa_l, gpsa_h]);

      translate([
        case_x - gpsa_w / 2 + wall_thickness - 5,
        gpsa_l + wall_thickness,
        wall_thickness
      ]) cube([10, wall_thickness, gpsa_h]);

      translate([
        case_x - gpsa_w,
        gpsa_l / 2 + wall_thickness - 5,
        wall_thickness
      ]) cube([wall_thickness, 10, gpsa_h]);
    }
  }
}


// Lid //
if (lid) {  
  translate([0, case_y + wall_thickness * 2 + 10]) {

    // Rounded corners //
    for (i = [0 : 1 : 1]) {
      difference() {

        translate([
          5 + i * (case_x + wall_thickness * 2), 5
        ]) hull() {
          
          cylinder(lid_thickness, d=5 * 2);
          
          translate([0, case_y - 10 + wall_thickness * 2])
            cylinder(lid_thickness, d=5 * 2);
        }

        translate([
          5 + i * (case_x + wall_thickness * 2 - 5), 0
        ]) cube([
          5, case_y + wall_thickness * 2,
          lid_thickness
        ]);
        

        // Screw holes //
        for (j = [0 : 1 : 1]) translate([
          i * (case_x + 5 + wall_thickness * 2 - 2) + 0.75,

          7.125 + j *
          (case_y - 20 + wall_thickness * 2), 0
        ]) screwHole();
      }
    }


    // Box for the lid //
    difference() {
      translate([5, 0]) cube([
        case_x + wall_thickness * 2,
        case_y + wall_thickness * 2,
        lid_thickness
      ]);


      // Text //
      translate([
        case_x / 2 + 5 + wall_thickness,
        case_y + wall_thickness * 2 - 2,
        wall_thickness / 2
      ]) rotate([180]) linear_extrude(wall_thickness / 2) 
        text(
          "NetLog MK1",
          6, font=text_font,
          halign="center",
          valign="bottom"
        );


      // More screw holes //
      for (i = [0 : 1 : 1]) for (j = [0 : 1 : 1]) translate([
        i * (case_x + 5 + wall_thickness * 2 - 2) + 0.75,

        7.125 + j *
        (case_y - 20 + wall_thickness * 2), 0
      ]) screwHole();
    }


    // Support for the thin mainboard wall //
    translate([5 + wall_thickness + tolerance_offset,
              tolerance_offset + wall_thickness / 2, lid_thickness])

              cube([mainb_w - tolerance_offset * 2, wall_thickness, 1]);


    translate([
      5 + wall_thickness,
      wall_thickness,
      lid_thickness
    ]) {
      
      // Holders for mainboard //
      for (i = [0 : 1 : 1]) translate([
        tolerance_offset + i * (mainb_w - wall_thickness - tolerance_offset * 2),
        case_y - 45 - tolerance_offset - (case_y - mainb_l)
      ]) cube([wall_thickness, 45, case_z - mainb_h]);


      // Holder for GPS antenna (+0.25mm height) //
      translate([
        case_x + tolerance_offset - gpsa_w,
        case_y + tolerance_offset - gpsa_l
      ]) cube([
        gpsa_w - tolerance_offset * 2,
        gpsa_l - tolerance_offset * 2,
        case_z - gpsa_h + 0.25
      ]);

      
      // Holders for GPS module (+0.25mm height) //
      for (i = [0 : 1 : 1]) for (j = [0 : 1 : 1]) translate([
        case_x - 5 - tolerance_offset - i * (gps_w - 5 - tolerance_offset * 2),
        tolerance_offset + j * (gps_l - 5 - tolerance_offset * 2),
      ]) cube([5, 5, case_z - gps_h + 0.25]);
    }
  }
}
