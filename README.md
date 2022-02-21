
 *  INSTRUCTIONS:
 * Create a program that helps with managing a square foot garden.
 
 * Refactor 9 - Jacob B. Seman

 *  MENU OPTIONS:
 * clear display area, ~20 lines, add more if planter exceeds height
 * print prompt/instructions/information
 * take input from user - new,save,load,add,change,remove,quit
 
 *  NEW:
 * create new empty garden
 * take input from user - garden name
 * this name is used to identify data
 
 *  SAVE:
 * push current garden/planters/plants configuration to data file
 * leave current garden state unchanged
 
 *  LOAD:
 * getlines from text file, prompt selections available
 * take data from text file per selection
 * populate and print saved garden/planters/plants
 * take input from user - save,load,new,add,change,remove,quit
 
 *  ADD:
 * add a planter or plant to the current garden
 * take input from user - planter:
 *                        - sizeX,sizeY
 *                      - plant:
 *                        - name,size,quantity
 * compute garden arrangement after new planter or plant
 * print updated garden
 * if a new planter sizeY exceeds 16, display height will be increased
 * accordingly
 
 *  CHANGE:
 * change a planter or plant currently in the garden
 * take input from user - change planter,change plant
 * take input from user - planter number:
 *                        - sizeX,sizeY
 *                      - plant name:
 *                        - size,name,quantity
 * compute garden arrangement after changing planter or plant
 * print updated garden
 
 *  REMOVE:
 * remove a planter or plant from the garden
 * take input from user - planter,plant
 * compute garden arrangement after removing planter or plant
 * print updated garden
 
 *  HELP:
 * toggles the introductory help screen
 * 
 *  OPTIONS:
 * configure the display height unless a larger planter is overriding this
 * choose a display character for plant fill
 
 *  QUIT:
 * exit program, prompt to save
 * take input from user - yes,no,cancel
 * save if yes, exit program unless cancel
