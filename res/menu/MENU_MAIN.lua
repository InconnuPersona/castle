local MENU_MAIN = {}

local selected = 0

function MENU_MAIN.setup()
 selected = 0
end

function MENU_MAIN.tick()
 if key.clicked(key.UP) then
  selected = selected - 1
 end
 
 if key.clicked(key.DOWN) then
  selected = selected + 1
 end
 
 if selected > 4 then
  selected = 0
 elseif selected < 0 then
  selected = 4
 end
 
 if key.clicked(key.ENTER) then
  if selected == 0 then
   menu.change(menu.LOBBY)
  elseif selected == 1 then
   menu.change(menu.NETWORK)
  elseif selected == 4 then
   menu.cueexit()
  end
 end
end

function MENU_MAIN.render()
 render.setcolor(0, 0, 0)
 render.flat(0, 0, render.WIDTH, render.HEIGHT)
 
 render.setcolor(255, 255, 255)
 render.setoffset(8, 0)
 render.chars(0, 16, "Main menu - welcome to castles!")
 
 render.chars(0, 3 * 16, "Start")
 render.chars(0, 4 * 16, "Network")
 render.chars(0, 5 * 16, "Options")
 render.chars(0, 6 * 16, "Adventure (secret)")
 render.chars(0, 7 * 16, "Quit")
 
 render.setoffset(0, 0)
 
 render.setcolor(255, 0, 0)
 render.box(1, 16 * (selected + 3), 8, 16)
end

return MENU_MAIN
