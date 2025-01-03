local sprite = app.activeSprite

array_size = 0
length_of_array = 0

-- Check constraints
if sprite == nil then
  app.alert("No Sprite...")
  return
end
if sprite.colorMode ~= ColorMode.INDEXED then
  app.alert("Sprite needs to be indexed")
  return
end

local function getPaletteData(palette)
	local ncolors = #palette
	local res = string.format("%i\n", ncolors)

	for i=0, ncolors-1 do
		local color = palette:getColor(i)
		res = res .. string.format("%i %i %i %i\n", color.red, color.green, color.blue, color.alpha)		
	end

	return res
end

local function getIndexData(img, x, y, w, h)
	local res = ""
	for y = 0,h-1,16 do
		for x = 0, w-1,16 do
			px = img:getPixel(x, y)
			res = res .. string.format("%i, ", px)
		end
		res = res .. "\n"
	end

	return res
end

local function calcArraySize(width, height)

    array_size = (width/16) * (height/16)
	length_of_array = width/16


    -- return (width * height)

end

local function exportFrame(frm, exportFile)
	if frm == nil then
		frm = 1
	end

	local img = Image(sprite.spec)
	img:drawSprite(sprite, frm)
	
    	calcArraySize(sprite.width, sprite.height)
	local filename = exportFile:match("([^/]+)%.%w+$")

	io.write("static const uint8_t ")
	io.write(filename)
	io.write("Collision[")
	io.write(array_size)    
	io.write("] =\n{\n")
	io.write(getIndexData(img, x, y, sprite.width, sprite.height))
	io.write("};\n") 
	
	io.write("static const uint16_t ")
	io.write(filename)
	io.write("TileWidth = ")
	io.write(sprite.width)
	io.write("/16;\n")
	io.write("static const uint16_t ")
	io.write(filename)
	io.write("TileHeight = ")
	io.write(sprite.height)
	io.write("/16;\n")
	
	io.write("static const uint16_t ")
	io.write(filename)
	io.write("MapWidth = ")
	io.write(sprite.width)
	io.write(";\n")
	io.write("static const uint16_t ")
	io.write(filename)
	io.write("MapHeight = ")
	io.write(sprite.height)
	io.write(";\n")
	io.write("\n")
end

local dlg = Dialog()
dlg:file{ id="exportFile",
          label="File",
          title="SGDK Export",
          open=false,
          save=true,
        --filename= p .. fn .. "pip",
          filetypes={"h", "pip" }}
dlg:check{ id="onlyCurrentFrame",
           text="Export only current frame",
           selected=true }

dlg:button{ id="ok", text="OK" }
dlg:button{ id="cancel", text="Cancel" }
dlg:show()

local data = dlg.data
if data.ok then
    
	local f = io.open(data.exportFile, "w")
	io.output(f)

	if data.onlyCurrentFrame then
		exportFrame(app.activeFrame, data.exportFile) 
	else
	 	for i = 1,#sprite.frames do
	 		io.write(string.format(";Frame %d\n", i))
	 		exportFrame(i, data.exportFile) 
		end
	end

	io.close(f)
end



