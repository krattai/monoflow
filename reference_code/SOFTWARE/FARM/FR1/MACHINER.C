int machinery(void)
{
   int key,
       button = 1,
       status = 0,
       h, v;
   machineplates();
   do{
      key = checkinput();
      h = rat.horiz;
      v = rat.verti;
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
      {
         clearplates();
         status = farmpower();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
      {
         clearplates();
         status = cropping();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
      {
         clearplates();
         status = haying();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
      {
         clearplates();
         status = specialcrops();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
      {
         clearplates();
         status = dairy();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
      {
         clearplates();
         status = swine();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(6,h,v)))
      {
         clearplates();
         status = poultry();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(7,h,v)))
      {
         clearplates();
         status = genlvstock();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(8,h,v)))
      {
         clearplates();
         status = misc();
         machineplates();
      }
      if((rat.buttons == LBUTTON && key == PRESS) && (platehit(9,h,v)))
      {
         clearplates();
         status = landlords();
         machineplates();
      }
      if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
      {
	     if(buton)
		 {
		    buttonoff(1);
			buton = 0;
		 }
		 else
		 {
		    buttonon(1);
			buton = 1;
		 }
	  }
     if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
      {
	     if(buton)
		 {
		    buttonoff(1);
			buton = 0;
		 }
		 else
		 {
		    buttonon(1);
			buton = 1;
		 }
	  }
     if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
     {
	    status = 1;
	 }
	 if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
     {
		status = 2;
     }
     if (status == 2 || status == 1) key = ESC;
     }while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER)) && checkhit(3,h,v) || (key == ESC));
        clearplates();
        return(status);
     }



     void machineplates(void)
     {
//        defplate(0,25,"Farm Power");
//        defplate(  ,  ,"General Cropping");
//        defplate(  ,  ,"Haying");
//        defplate(  ,  ,"Special Crops");
//        defplate(  ,  ,"Dairy and Beef");
//        defplate(  ,  ,"Swine Equipment");
//        defplate(  ,  ,"Poultry Equipment");
//        defplate(  ,  ,"General Livestock");
//        defplate(  ,  ,"Miscellaneous");
//       defplate(  ,  ,"LandLords");
     }