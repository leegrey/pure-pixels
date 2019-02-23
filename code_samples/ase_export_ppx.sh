echo "___________________________________________________________"
echo ""
echo "Exporting..."
echo "___________________________________________________________"
echo ""

## NOTE: depending on where you run the script from, you may need to
## add a bunch of fiddly relative path markers: ../

../tools/osx/aseprite/aseprite -b --split-layers ./art/pure_level1.ase --save-as ./build/assets/world1/world_1_.png

# rename the files because Aseprite's default naming is weird 
# (wrapped in doublequotes because of space in aseprite naming)
mv "build_osx/assets/world1/world_1_ (foreground).png" build/assets/world1/foreground.png
mv "build_osx/assets/world1/world_1_ (solid).png" build/assets/world1/solid.png
mv "build_osx/assets/world1/world_1_ (background_objects).png" build/assets/world1/background_objects.png
mv "build_osx/assets/world1/world_1_ (background).png" build/assets/world1/background.png