/*
 *  Document:         Whets.java 
 *  File Group:       Classic Benchmarks
 *  Creation Date:    22 January 1997
 *  Revision Date:    
 *
 *  Title:            Whetstone Benchmark Java Version
 *  Keywords:         WHETSTONE BENCHMARK PERFORMANCE MIPS
 *                    MWIPS MFLOPS JAVA
 *
 *  Abstract:         Web/Java version of Whetstone one of the
 *                    Classic Numeric Benchmarks.        
 *
 *  Contributor:      Roy_Longbottom@compuserve.com
 *
 *           Copyright (C) 1997, Roy Longbottom
 *
 ********************
 *
 *          Java Whetstone Benchmark Single Precision
 *
 *     Original concept        Brian Wichmann NPL      1960's
 *     Original author         Harold Curnow  CCTA     1972
 *     Self timing versions    Roy Longbottom CCTA     1978/87
 *     Optimisation control    Bangor University       1987/90
 *     C/C++ and PC Versions   Roy Longbottom          1996
 *     Java version            Roy Longbottom          1997
 *
 ********************
 *
 *     The program normally runs for about 100 seconds
 *     (adjustable duration in initial parameters). This time
 *     is necessary because of poor PC clock resolution.
 *            
 ********************
 *
 *     Descriptions of history and changes in the benchmark
 *     can be found in WHETS.C and WHETSOLD.TXT available
 *     in the Classics Benchmark Library at the CompuServe
 *     Benchmark and Standards Forum.
 *
 *     Changes for this version are described in WHETS.HTML.
 *     This is required for driving WHETS.CLASS as produced
 *     when this program is compiled.
 *
 *     Examples of numeric results and performance on various
 *     PCs are also given in WHETS.HTML. The latter can also
 *     be obtained from the Benchmark and Standards Forum.
 *
 *              Roy_Longbottom@compuserve.com
 *
 ********************
 */

import java.applet.*;
import java.awt.*;


public class Whets extends Applet
{
	private double TimeUsed;
	private float x,y,z;
	private float[] e1 = new float[4];
	private float Check;
	private float mwips;
	private float[] loop_time = new float[9];
	private float[] loop_mops = new float[9];
	private float[] loop_mflops = new float[9];
	private float[] results = new float[9];
	private String[] headings = new String[9];
	
	

	private int count = 11;
	private int xtra = 1;
	private int x100 = 100;
	private int duration = 100;	 // Benchmark running time
	private int calibrate = 1;
	private int routine;

	private int pausetime;
	private int wwidth = 500;
	private int wheight	= 325;
	private int ypos1 = 130;
	private int ycal = 130;
    private int ypos2 = 300;
	private int yinc1 = 17;
	private int ypos3 = 140;
	private int yms = 50;
	private int boxtop = 27;
	private int boxhigh = 40;
	private int xbox = 55;
	private int boxwide = 390;

	private int pixelrate = 0;
	private String xout;
	private int whatToDisplay = -1;

	Font font1;
	Font font2;
	Font font3;
	Font font4;
	Font font5;

	private Button runButton = new Button ("Run");

	public void paint(Graphics g)
	{
		int i, j;
		double howlong;

		String calib = " ";
		String movin = " ";

		if (whatToDisplay == 0)
		{
			removeAll();
		}

		displayAll(g);

		if (count == 10)
		{
			g.setFont(font4);
			g.setColor(Color.blue);			
			
			g.drawString("Welcome to the benchmark", xbox + 45, boxtop + 25);
							  
			do
			{
				Check=0.0f;
				TimeUsed = 0;

				for (routine = 1; routine < 9; routine ++)
				{
				   whetstones(xtra, x100, calibrate);
				}

				calib = "                    Calibrating " + TimeUsed +
					         " Seconds " + xtra + " (x" + duration + ") Passes            ";

				g.setFont(font3);
				g.setColor(Color.white);
				for (j = 0; j < 10; j++)
				{
					g.drawString(calib, 5, ycal + calibrate * yinc1);
				}

				calibrate ++;
				count--;

				if (TimeUsed > 2.0)
				{
					count = 0;
				}
				else
				{
					xtra = xtra * 5;
				}
			}
			while (count > 0);

			if (TimeUsed > 0)
			{
				xtra = (int)((float)(duration * xtra) / (float) TimeUsed);
			}
			if (xtra < 1)
			{
				xtra = 1;
			}
			
			calibrate = 0;

		}

		if (count == 0)
		{
			Check=0.0f;
			TimeUsed = 0;
			whatToDisplay = 1;

 			movin = "Sorry you have to wait";
			for (routine = 1; routine < 9; routine ++)
			  {
			      whetstones(xtra, x100, calibrate);
				  
				  displayAll(g);
				  g.setColor(Color.black);
				  g.fillRect(xbox, boxtop, boxwide, boxhigh);

				  g.setFont(font4);
				  g.setColor(Color.green);

				  howlong = (double) duration - TimeUsed;
				  if (howlong < 0)
				  {
					  howlong = 0;
				  }
				  if (howlong < 0.7 * (double) duration)
				  {
					g.setColor(Color.cyan);
					movin = "Still another " + (int) howlong + " seconds"; 
				  }
				  if (howlong < 0.3 * (double) duration)
				  {
					 g.setColor(Color.green);
					 movin = "   Not long to go now";
				  }
				  g.drawString(movin, xbox + 70, boxtop + 35);

				  g.setColor(Color.yellow);
				  g.setFont(font3);
				  g.drawString("Running time about " + duration +
					  		       " seconds " + xtra +
							       " (x" + duration +") passes", xbox + 20, boxtop + 14);
				
				  for (j = 0; j < 10; j++)
				  {
					  g.copyArea(xbox, boxtop, boxwide, boxhigh, 0, 0);
				  }
			  }

			if (TimeUsed > 0)
			{
				mwips = (float)xtra * (float)x100 / (float)(10.0 * TimeUsed);
			}
			else
			{
				mwips = 0;
			}
			count = -1;
			routine = 8;
		}


		if (whatToDisplay == 1)
		{
			
			displayAll(g);


			double time1 = dtime();
			double time2 = 0;
			int passcount = 0;
			pausetime = 0;

			movin = "OUCH! That Was Slow";

			if (mwips > 90)
			{
				movin = "WOW! That Was Fast.";
			}
			else if (mwips > 35)
			{
				movin = "GEE! Not Bad Speed.";
			}			
			else if (mwips > 10)
			{
				movin = "MMM! Not That Fast.";
			}
			else if (mwips > 4)
			{
			    movin = "OH NO! Pretty Slow.";
			}
								  			
			g.setFont(font2);
			g.setColor(Color.red);			
			do
			{
				g.drawString(movin, xbox + 25, boxtop + 30);
			}
			while (dtime() - time1 < 3);

  			g.setColor(Color.white);
			g.fillRect(xbox, boxtop, boxwide, boxhigh);

			g.setFont(font2);
			g.setColor(Color.red);

			time1 = dtime();
			do
			{
				for (i=0; i<boxwide; i++)
				{
					g.copyArea(xbox + 1, boxtop, boxwide - 1, boxhigh, -1, 0);
				}
				g.drawString(movin, xbox + 25, boxtop + 30);
				passcount = passcount + 1;
			}
			while (dtime() - time1 < 2);
			time2 = dtime() - time1;

			pixelrate = (int)( (double) boxwide *
			                   (double) (boxwide - 1) * 
							   (double) boxhigh  *
							   (double) passcount /
								time2 / 1000000.0);

			movin = "OUCH! That Was Slow";

			if (pixelrate > 120)
			{
				movin = "WOW! That Was Fast.";
			}
			else if (pixelrate > 65)
			{
				movin = "GEE! Not Bad Speed.";
			}			
			else if (pixelrate > 20)
			{
				movin = "MMM! Not That Fast.";
			}
			else if (pixelrate > 10)
			{
			    movin = "OH NO! Pretty Slow.";
			}

			whatToDisplay = 2;
			displayAll(g);

			g.setColor(Color.black);
			g.fillRect(xbox, boxtop, boxwide, boxhigh);
			g.setFont(font2);
			g.setColor(Color.magenta);
			for (j = 0; j < 10; j++)
			{
				g.drawString(movin, xbox + 25, boxtop + 30);
			}
			
			pausetime = 2;
			movedata(g);

			displayAll(g);

			movin = "Produced by Roy Longbottom";
			g.setColor(Color.black);
			g.fillRect(xbox, boxtop, boxwide, boxhigh);
			g.setFont(font4);		
			g.setColor(Color.cyan);
			for (j = 0; j < 20; j++)
			{
				g.drawString(movin, xbox + 40, boxtop + 25);
			}
			pausetime = 3;
			movedata(g);

			whatToDisplay = 3;
			displayAll(g);
	
		}

	}

	public void displayAll(Graphics g)
	{
		int i, j;
		int ypos1a;
 
		if (whatToDisplay == -1)
		{
			g.setFont(font3);
			g.setColor(Color.white);
																					 
			g.drawString("                                 WHETSTONE BENCHMARK", 5, yms);				  
			g.drawString("     Before running this benchmark, ensure that all other applications", 5, yms + 2 * yinc1 );
			g.drawString("     and windows are closed. The browser window should preferably be", 5, yms +  3 * yinc1);			
			g.drawString("    maximized and, when running, the mouse should not be moved (much).", 5, yms + 4 * yinc1);
			g.drawString("                 Also, the window should not be resized or scrolled.", 5, yms + 5 * yinc1);
			for (j = 0; j < 10; j++)
			{
			g.drawString("        Click on the Run button to execute the Whetstone benchmark. ", 5, yms + 7 * yinc1);
			}
		}
		if (whatToDisplay > -1)
		{
			g.setColor(Color.blue);
			g.fillRect(0, 0, wwidth, wheight);

			g.setFont(font5);
			g.setColor(Color.yellow);			
			g.drawString("Copyright (C) 1997, Roy Longbottom ", 150, wheight - 5);
		
			g.setColor(Color.yellow);	
			g.fillOval(xbox - 25, boxtop - 25, boxwide + 50, boxhigh + 50);

			g.setColor(Color.blue);
			g.fillRect(0, boxtop, wwidth, boxhigh);

			g.setColor(Color.yellow);
			g.fillRect(xbox - 4, boxtop , boxwide + 8, boxhigh);

			g.setColor(Color.blue);
			g.setFont(font3);
			g.drawString("WHETSTONE BENCHMARK", 150, boxtop - 5);		
		
			g.setFont(font4);

			g.drawString("1972", 225, boxtop + boxhigh + 20);

			g.setFont(font5);
	
			g.drawString("Original Version", 130, boxtop + boxhigh + 15);
			g.drawString("Harold Curnow", 280, boxtop + boxhigh + 15);

			g.setColor(Color.white);
			g.fillRect(xbox, boxtop, boxwide, boxhigh);				
		}
		if (whatToDisplay > 0)
		{
			g.setFont(font1);
			g.setColor(Color.white);
			
			for (i = 1; i < routine + 1; i++)
			{
			   ypos1a = ypos3 + yinc1 * i;
			   g.drawString(" " + headings[i], 5,ypos1a);
			   xout = format(results[i],3,9);
			   g.drawString(" " + xout, 140,ypos1a);
			   if (loop_mflops[i] < 9999)
			   {
				   xout = format(loop_mflops[i],5,3);
				   g.drawString(" " + xout, 265,ypos1a);
			   }
			   if (loop_mops[i] < 9999)
			   {
				   xout = format(loop_mops[i],5,3);
			       g.drawString(" " + xout, 335,ypos1a);
			   }
			   xout = format(loop_time[i],3,3);
			   g.drawString(" " + xout + "   ", 415, ypos1a);
			}

			
			g.drawString(" Loop content", 5, ypos1);
			g.drawString(" Result", 155,ypos1);
			g.drawString(" MFLOPS", 260, ypos1);
			g.drawString("   MOPS", 340, ypos1);

			for (j = 0; j < 10; j++)
			{
				g.drawString(" Seconds", 410, ypos1);
			}

			if (routine == 8)
			{
				g.setColor(Color.white);
				g.drawString(" MWIPS", 5,ypos2);
				xout = format(mwips,5,3);
				g.drawString(" " + xout, 265,ypos2);
				xout = format((float)TimeUsed,3,3); 
				g.drawString(" " + xout + "   ", 415, ypos2);
			}
			if (Check == 0)
			{
				g.drawString(" Wrong answer ", 5, ypos2 + yinc1);
			}

		}
		if (whatToDisplay > 1)
		{
			g.setColor(Color.yellow);
			g.setFont(font3);
			g.drawString("   Graphics area copy at " + pixelrate + 
			         " million pixels per second", xbox + 5, boxtop + boxhigh + 40);
		}
		if (whatToDisplay > 2)
		{
			g.setFont(font5);
			g.setColor(Color.blue);
			g.drawString(" Note results before closing your browser. " +
				         "You can take a copy using the", xbox + 3, boxtop + 14);
			g.drawString("  Print Screen key, then paste" +
				         " the Clipboard contents into a document.", xbox + 3, boxtop + 30);
		}

		return;
	}

	public void movedata(Graphics g)
	{
		double time1 = dtime();
		int i;
		
		do
		{
			g.copyArea(xbox + 1, boxtop, boxwide - 1, boxhigh, 0, 0);
		}
		while (dtime() - time1 < pausetime);

		time1 = dtime();

		for (i=0; i<boxwide; i++)
		{
			do
			{
				g.copyArea(xbox + 1, boxtop, boxwide - 1, boxhigh, 0, 0);			
			}
			while (dtime() - time1 < .03);
			time1 = dtime();
			g.copyArea(xbox + 1, boxtop, boxwide - 1, boxhigh, -1, 0);
		}
		return;
	}

	public Whets()
	{
	}

	public void whetstones(int xtra, int x100, int calibrate)
	{
		
		double timea, timeb;
		int n1,n2,n3,n4,n5,n6,n7,n8,i,ix,n1mult;              
		int j,k,l;
                        
		float t =  0.49999975f;
		float t0 = t;        
		float t1 = 0.50000025f;
		float t2 = 2.0f;
               
       
		n1 = 12*x100;
		n2 = 14*x100;
		n3 = 345*x100;
		n4 = 210*x100;
		n5 = 32*x100;
		n6 = 899*x100;
		n7 = 616*x100;
		n8 = 93*x100;

		switch (routine)
		{

		case 1:

			n1mult = 10;

			/* Section 1, Array elements */

			e1[0] =  1.0f;
			e1[1] = -1.0f;
			e1[2] = -1.0f;
			e1[3] = -1.0f;			 
			
			timea = dtime();
			{
				for (ix=0; ix<xtra; ix++)
				{
					for(i=0; i<n1*n1mult; i++)
					{
						e1[0] = (e1[0] + e1[1] + e1[2] - e1[3]) * t;
						e1[1] = (e1[0] + e1[1] - e1[2] + e1[3]) * t;
						e1[2] = (e1[0] - e1[1] + e1[2] + e1[3]) * t;
						e1[3] = (-e1[0] + e1[1] + e1[2] + e1[3]) * t;
					}
					t = 1.0f - t;
				}
				t =  t0;                    
			}
			timeb = (dtime()-timea)/(float)(n1mult);
			pout("N1 floating point\0",(float)(n1*16)*(float)(xtra),
                             1,e1[3],timeb,calibrate,1);
			break;

		case 2:

			/* Section 2, Array as parameter */

			timea = dtime();
			{
				for (ix=0; ix<xtra; ix++)
				{ 
					for(i=0; i<n2; i++)
					{
						 pa(e1,t,t2);
					 }
					t = 1.0f - t;
				}
				t =  t0;
			}
			timeb = dtime()-timea;
			pout("N2 floating point\0",(float)(n2*96)*(float)(xtra),
                             1,e1[3],timeb,calibrate,2);
			break;

		case 3:

			/* Section 3, Conditional jumps */
			j = 1;
		    timea = dtime();
			 {
			     for (ix=0; ix<xtra; ix++)
		          {
				    for(i=0; i<n3; i++)
					  {
						if(j==1)       j = 2;
						else           j = 3;
						if(j>2)        j = 0;
						else           j = 1;
                        if(j<1)        j = 1;
                        else           j = 0;
				      }
				  }
			 }
			timeb = dtime()-timea;
			pout("N3 if then else  \0",(float)(n3*3)*(float)(xtra),
                        2,(float)(j),timeb,calibrate,3);
			break;

		case 4:

			/* Section 4, Integer arithmetic */
	        j = 1;
		    k = 2;
		    l = 3;
	       timea = dtime();
		     {
			    for (ix=0; ix<xtra; ix++)
				  {
					for(i=0; i<n4; i++)
					{
						j = j * (k-j) * (l-k);
						k = l * k - (l-j) * k;
				        l = (l-k) * (k+j);
						e1[l-2] = j + k + l;
						e1[k-2] = j * k * l;
					}
				  }
			}
			timeb = dtime()-timea;
			x = e1[0]+e1[1];
			pout("N4 fixed point   \0",(float)(n4*15)*(float)(xtra),
                                 2,x,timeb,calibrate,4);
			break;

		case 5:
     
			/* Section 5, Trig functions */
			x = 0.5f;
			y = 0.5f;
		   timea = dtime();
			 {
				for (ix=0; ix<xtra; ix++)
				{
					for(i=1; i<n5; i++)
					{
						 x = (float)(t*Math.atan(t2*Math.sin(x)*Math.cos(x)/
							 (Math.cos(x+y)+Math.cos(x-y)-1.0)));
						y = (float)(t*Math.atan(t2*Math.sin(y)*Math.cos(y)/
							 (Math.cos(x+y)+Math.cos(x-y)-1.0)));
					}
					t = 1.0f - t;
				}
				t = t0;
			 }
			timeb = dtime()-timea;
			pout("N5 sin,cos etc.  \0",(float)(n5*26)*(float)(xtra),
                                 2,y,timeb,calibrate,5);
			break;

		case 6:
  
			/* Section 6, Procedure calls */
			x = 1.0f;
			y = 1.0f;
			z = 1.0f;
			timea = dtime();
			{
				for (ix=0; ix<xtra; ix++)
				{
					for(i=0; i<n6; i++)
					{
						 p3(t,t1,t2);
					}
				}
			}
			timeb = dtime()-timea;
			pout("N6 floating point\0",(float)(n6*6)*(float)(xtra),
                                1,z,timeb,calibrate,6);
			break;

		case 7:
  
			/* Section 7, Array refrences */
			j = 0;
			k = 1;
			l = 2;
			e1[0] = 1.0f;
			e1[1] = 2.0f;
			e1[2] = 3.0f;
			timea = dtime();
			{
				for (ix=0; ix<xtra; ix++)
				{
					for(i=0;i<n7;i++)
					{
						 po(e1,j,k,l);
					}
				}
			}
			timeb = dtime()-timea;
			pout("N7 assignments   \0",(float)(n7*3)*(float)(xtra),
                            2,e1[2],timeb,calibrate,7);
			break;

		case 8:
        
			/* Section 8, Standard functions */
			x = 0.75f;
			timea = dtime();
			{
				for (ix=0; ix<xtra; ix++)
				{
					for(i=0; i<n8; i++)
					{
						x = (float)(Math.sqrt(Math.exp(Math.log(x)/t1)));
					}
				}
			}
			timeb = dtime()-timea;
			pout("N8 exp,sqrt etc. ",(float)(n8*4)*(float)(xtra),
                          2,x,timeb,calibrate,8);
			break;
		}

	}

      void pa(float e[], float t, float t2)
      {
         long j;
         for(j=0;j<6;j++)
            {
               e[0] = (e[0]+e[1]+e[2]-e[3])*t;
               e[1] = (e[0]+e[1]-e[2]+e[3])*t;
               e[2] = (e[0]-e[1]+e[2]+e[3])*t;
               e[3] = (-e[0]+e[1]+e[2]+e[3])/t2;
            }

         return;
      }

    void po(float e1[], int j, int k, int l)
      {
         e1[j] = e1[k];
         e1[k] = e1[l];
         e1[l] = e1[j];
         return;
      }

    void p3(float t, float t1, float t2)
      {
         x = y;
         y = z;
         x = t * (x + y);
         y = t1 * (x + y);
         z = (x + y)/t2;
         return;
      }

    void pout(String title, float ops, int type, float checknum,
              double time, int calibrate, int section)
      {
		float mops, mflops;

		Check = Check + checknum;
        loop_time[section] = (float) time;
		headings[section] = title;
		TimeUsed =  TimeUsed + time;       
        
        if (calibrate == 1)
     
          {
              results[section] = checknum;
          }
        if (calibrate == 0)
          {              
            if (type == 1)
             {
                if (time>0)
                 {
                    mflops = ops/(1000000L*(float)time);
                 }
                else
                 {
                   mflops = 0;
                 }
                loop_mops[section] = 99999;
                loop_mflops[section] = mflops;
             }
            else
             {
                if (time>0)
                 {
                   mops = ops/(1000000L*(float)time);
                 }
                else
                 {
                   mops = 0;
                 }
                loop_mops[section] = mops;
                loop_mflops[section] = 9999;                 
             }

		  }  
        return;
      }

    public String format(float x, int v, int d)
	{
		String s = "";
		String s2;
		int j, len, p;
		int i = (int) Math.abs((double)x);
		float r = (float) Math.abs((double)x) - (float) i;
		String m = " ";
		if (x < 0)
		{
			m = "-";
		}
		s2 = m + i;
		len = v - s2.length();
		if (len < 1)
		{
			s = s2;
		}
		else
		{
			for (j = 0; j < len; j++)
			{
				s = s + " ";
			}
			s = s + s2;
		}
		s = s + ".";
		p = (int)(r * Math.pow(10d,(double)d));
		s2 = "" + p;
		len = d - s2.length();
		if (len < 1)
		{
			s = s + s2;
		}
		else
		{
			for (j = 0; j < len; j++)
			{
				s = s + "0";
			}
			s = s + s2;
		}

		return s;
	}


	public double dtime()
	{
		double q;
		long itime;
		itime = System.currentTimeMillis();
		q = (double) itime;
		return q / 1000.0;
	}

	public String getAppletInfo()
	{
		return "Name: Whets\r\n" +
		       "Author: R Longbottom\r\n" +
		       "Created with Microsoft Visual J++ Version 1.0";
	}


	public void init()
	{
		resize(wwidth, wheight);

		add (runButton);

        font1 = new Font("CourierNew", Font.BOLD, 14);
		font2 = new Font("TimesRoman", Font.BOLD, 36);
		font3 = new Font("TimesRoman", Font.BOLD, 16);
		font4 =	new Font("TimesRoman", Font.BOLD, 24);
		font5 =	new Font("TimesRoman", Font.BOLD, 12);
	
        setBackground(Color.blue);
	}

	public boolean action(Event pressButton, Object obj)
	{
		Object obPress = pressButton.target;
		if (obPress instanceof Button)
		{
			count = 10;
			whatToDisplay = 0;
			repaint();
			return true;
		}
		return false;
	}


	public void destroy()
	{
	}


	public void start()
	{
	}
	
	public void stop()
	{
	}



}