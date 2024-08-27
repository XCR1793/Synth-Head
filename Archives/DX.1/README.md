# Synth Head DX.1
Building off of the DX base, many more components were added. This version of the DX series was to brainstorm, test and prototype if the shell worked IRL along with with features could be theoretically added. I stayed in this version for a while when developing the physical design and concidering alternative restraint mechanisms. The [manifesto](/README.md#synth-head) when constructing this remained. Although this version does have a physical build to it, <strong style='color: #ff0000;'>IT IS NOT RECOMMENDED</strong> that you construct this without your modifications due to some known design issues.

## Known Issues
* Neck Plate Attachment to head is missing as the plates were an after thought.
* Neck Plate Attachment to each other is also missing as the plates were designed independantly of each other and near the end of the version's use.
* Head Restraint not providing enough pressure was determined after construction as the realisation that there was no tensioning mechanism other then just manually pulling the steel zip tie.
* No way to properly attach the tongue as this was designed during construction.
* Neck Plate Tensioning is lacking as the rubber bands that were used barely had enough strength to prop themselves up.
* Unconfortable Jaw Joint design which was only detected after constructing and during the testing stages.

## File Structure
<strong style='color: #bb99ff;'>XCR Head DX.1.f3d</strong> : A fusion 360 file for the main project where the entirity of DX.1 was designed.

<strong style='color: #bb99ff;'>XCR Head DX.1.step</strong> : A step file version incase you dont have fusion 360 to open the main project.

<strong style='color: #bb99ff;'>Rubber Holder V3.step</strong> : A step file of the rubber band holder reference.

<strong style='color: #bb99ff;'>Subsurface Jaw Joint V3.step</strong> : A step file of the jaw joint v3 mechanism.

<strong style='color: #bb99ff;'>Ziptie Release & Back Brace.step</strong> : A step file of the steel cable tie head restraint assembly.

<strong style='color: #bb99ff;'>Visor Mesh.step</strong> : A step file for the Mesh Visor addon incase you cant vacuum form your own visor.

<strong style='color: #bb99ff;'>DX Visor Buck.f3d</strong> : A f3d file for the visor buck to vacuum form.

<strong style='color: #bb99ff;'>DX Visor Buck.step</strong> : A step file for the visor buck to vacuum form incase you dont have fusion 360.

## Features & Design Considerations
### DX
All features from the DX base design are included except the Jaw Joint and a modification to the head restraint.

### Jaw Joint Upgrade
The jaw hinge mechanism was upgraded from V2 to V3 which allows a slimmer mating surface which connects the jaw to the shell. This new version does not require a teflon tube to be inserted to lock the hinge into place, rather it is a press and turn system where the joint is designed to be integrated below the surface of the shell rather then on top. This allows the upgrade to sit more flush within the shell.

### Foam Mount for Head Restraint
As hotglue is not always available to the user, an inset slot was designed to grip the foam on its way in rather than with any other adhesives, preventing the foam from being destroyed due to hotglue and granting the user to easily change it out.

### Steel Cable Tie Head Restraint
As the base design offered no tensioning or size adjustment mechanism, a slidable adjuster was created via leveraging the use of a steel cable tie. Slight modifications to the actual cable tie sliding system was required to allow the cable tie to actually unlock.

### Steel Cable Tie Neck Plate Mounts
The springy ness of the steel cable tie was also experimented here by attaching the neck plates to them which automatically averaged their positions like a spline, which proved incredibly useful as the rubber bands on their own could not do the job.

### Slottable Top Maw Plate
An upgrade to the maw plate also was required however, it still did required the user to tack weld the piece into place as there wasn't any system that prevent it from just popping back out.

### Jaw Rubber Band Tensioning
Using the V3 rubber band mount proved to be a worthy upgrade as it prevented the sharp edges of the previous design from scratching the user during its motion. It is required for the jaw to function as there is no mechanism that prevents the jaw from slacking open.

### TPU Neck Plate Mounts
In addition to the steel cable tie mounts for the neck plates, a tpu version was also tested. This design allowed fixed equal spacing between the scales plates whilst also being flexible enough to bend to the user's movements.

### Mesh Visor Addon
If you dont have access to a vacuum former then fret not, I have also provided a mesh version of the visor which you can 3D print or cut out that should confirm roughly to the shape of the original visor.