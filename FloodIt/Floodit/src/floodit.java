import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.*;

import javax.swing.*;

@SuppressWarnings("serial")
class Stage extends JPanel
{
	final int width = 700, height = 700;
	public Stage() 	{
	}
	@Override
	public Dimension getPreferredSize() {
		return new Dimension(this.width,this.height);								//set Canvas size
	}
	@Override
	protected void paintComponent(Graphics g) 
	{
		super.paintComponent(g);
		g.clearRect(0, 0, this.width, this.height);									//Canvas Clear
	}

}

@SuppressWarnings("serial")
public class floodit extends JFrame 
{
	final int BLUE = 0, GREEN = 1, CYAN = 2, RED = 3, MAGENTA = 4, YELLOW = 5;
	
	
	private Stage stage = new Stage();
	
	/* add more variable */
	
	final int blockSize = 50;														//Cell Size
	
	private int clickCnt = 0;														//Counter variable
	private JLabel msg = new JLabel("Counter : " + clickCnt);						//Counter Message
	
	
	//....
	
	int SpaceMax = 14;
	private char map[][] = new char[SpaceMax][SpaceMax];
	private int mapKari[][] = new int[SpaceMax][SpaceMax];
	char CurrentColor;
	char ChangeColor;
	
	/* end */
	
	
	public floodit() 
	{	
		/* window layout setting */
		this.setLocation(300, 100);													//window position
		this.setTitle("Flood it 14x14 6-color");									//window title
		
		this.setLayout(new GridBagLayout());										//Set layout Configuration
		GridBagConstraints cst = new GridBagConstraints();

		/* "Counter Label" */
		cst.fill = GridBagConstraints.HORIZONTAL;									//set "Counter Label" layout
		cst.gridx = 0;		cst.gridy = 0;										
		cst.gridwidth = 1;	
		this.add(msg, cst);																													
		/* "Counter Label" end */
		
		
		/* Button setting */
		/* Blue button */
		JButton blueBtn = new JButton("Blue");										//make "Blue" button object
		blueBtn.addMouseListener(new MouseAdapter()									//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				btnClicked(BLUE);
			}
		});
		blueBtn.setBackground(Color.BLUE);  										//set "Blue" button layout 
		cst.fill = GridBagConstraints.HORIZONTAL;
		cst.gridx = 0;	cst.gridy = 1;									
		this.add(blueBtn,cst);			
		/* Blue Button end */
		
		
		/* Green Button */
		JButton greenBtn = new JButton("Green");									//make "Green" button object
		greenBtn.addMouseListener(new MouseAdapter()								//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				btnClicked(GREEN);
			}
		});
		greenBtn.setBackground(Color.GREEN);										//set "Green" button layout
		cst.fill = GridBagConstraints.HORIZONTAL;
		cst.gridx = 1;
		cst.gridy = 1;
		this.add(greenBtn,cst);
		/* Green Button end */
		
		
		/* Cyan Button */
		JButton CyanBtn = new JButton("Cyan");										//make "Cyan" button object
		CyanBtn.addMouseListener(new MouseAdapter()									//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				btnClicked(CYAN);
			}
		});
		CyanBtn.setBackground(Color.CYAN);		  									//set "Cyan" button layout
		cst.fill = GridBagConstraints.HORIZONTAL;		
		cst.gridx = 0;									
		cst.gridy = 2;									
		this.add(CyanBtn,cst);			
		/* Cyan Button end */
		
		/* Red Button */
		JButton RedBtn = new JButton("Red");										//make "Red" button object
		RedBtn.addMouseListener(new MouseAdapter()									//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				btnClicked(RED);
			}
		});
		RedBtn.setBackground(Color.RED);											//set "Red" button layout
		cst.fill = GridBagConstraints.HORIZONTAL;
		cst.gridx = 1;
		cst.gridy = 2;
		this.add(RedBtn,cst);
		/* Red Button end */
		
		/* Magenta Button */
		JButton MagentaBtn = new JButton("Magenta");								//make "Magenta" button object
		MagentaBtn.addMouseListener(new MouseAdapter()								//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				btnClicked(MAGENTA);
			}
		});
		MagentaBtn.setBackground(Color.MAGENTA);  									//set "Magenta" button layout
		cst.fill = GridBagConstraints.HORIZONTAL;		
		cst.gridx = 0;									
		cst.gridy = 3;									
		this.add(MagentaBtn,cst);			
		/* Magenta Button end */
		
		/* Yello Button */
		JButton YellowBtn = new JButton("Yellow");									//make "Yellow" button object
		YellowBtn.addMouseListener(new MouseAdapter()								//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				btnClicked(YELLOW);
			}
		});
		YellowBtn.setBackground(Color.YELLOW);										//set "Yellow" button layout
		cst.fill = GridBagConstraints.HORIZONTAL;
		cst.gridx = 1;
		cst.gridy = 3;
		this.add(YellowBtn,cst);
		/* Yellow Button end*/
		
		/* input Button */
		JButton inputBtn = new JButton("input");									//make "Yellow" button object
		inputBtn.addMouseListener(new MouseAdapter()								//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				inputprocess();
			}
		});
		inputBtn.setBackground(Color.white);										//set "Yellow" button layout
		cst.fill = GridBagConstraints.HORIZONTAL;
		cst.gridx = 0;
		cst.gridy = 4;
		this.add(inputBtn,cst);
		/*  Button end*/
		
		
		cst.gridx = 2;cst.gridy = 0;cst.gridheight = 5;cst.insets = new Insets(3, 3, 3, 3);	
		this.add(this.stage,cst);


		this.pack();
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
		
		@SuppressWarnings("unused")
		Graphics g = this.stage.getGraphics();

	}
	
private void inputprocess(){
	File file;
	FileReader filereader;
		
	try {
		file = new File("input.txt");
		filereader = new FileReader(file);
		BufferedReader br = new BufferedReader(filereader);
		
		String str; 
		
		
		
		for(int i = 0; i<SpaceMax; i++){
			str = br.readLine();
			for(int j = 0; j<SpaceMax; j++){
				
				map[i][j] = str.charAt(j);
				
				
			}

		}
		
		Graphics g = this.stage.getGraphics();
		
		for(int i = 0; i<SpaceMax; i++){
			for(int j = 0; j<SpaceMax; j++){
				if(map[i][j] == 'B'){
					
					g.setColor(Color.BLUE);
					g.fillRect(j*50, i*50, this.blockSize, this.blockSize);
				}
				
				else if(map[i][j] == 'G'){
					
					g.setColor(Color.GREEN);
					g.fillRect(j*50, i*50, this.blockSize, this.blockSize);
				}
				
				else if(map[i][j] == 'C'){
					
					g.setColor(Color.CYAN);
					g.fillRect(j*50, i*50, this.blockSize, this.blockSize);
				}
				
				else if(map[i][j] == 'R'){
					
					g.setColor(Color.RED);
					g.fillRect(j*50, i*50, this.blockSize, this.blockSize);
				}
				
				else if(map[i][j] == 'M'){
					
					g.setColor(Color.MAGENTA);
					g.fillRect(j*50, i*50, this.blockSize, this.blockSize);
				}
				
				else if(map[i][j] == 'Y'){
					
					g.setColor(Color.YELLOW);
					g.fillRect(j*50, i*50, this.blockSize, this.blockSize);
				}
			}
		}
		clickCnt = 0;
		this.msg.setText("Counter : " + (clickCnt));	
		
		
		CurrentColor = map[0][0];
		
		
		
	} catch (FileNotFoundException e1) {
		e1.printStackTrace();
	} catch (IOException e1) {
		 e1.printStackTrace();
	} catch (Exception e1) {
		e1.printStackTrace();
	} 	
		
		
		
			
}


	private void ChangeMap(int x, int y){
		if(y+1<SpaceMax)
			if(CurrentColor == map[x][y+1]){
				
				map[x][y+1] = ChangeColor;
				mapKari[x][y+1] = 1;
				ChangeMap(x, y+1);
			} 
		
		if(x+1<SpaceMax)
			if(CurrentColor == map[x+1][y]){
				
				map[x+1][y] = ChangeColor;
				mapKari[x+1][y] = 1;
				ChangeMap(x+1, y);
						
			} 
		
		if(x-1>=0)
			if(CurrentColor == map[x-1][y]){
				
				map[x-1][y] = ChangeColor;
				mapKari[x-1][y] = 1;
				ChangeMap(x-1, y);
						
			} 
		
		if(y-1>=0)
			if(CurrentColor == map[x][y-1]){
				
				map[x][y-1] = ChangeColor;
				mapKari[x][y-1] = 1;
				ChangeMap(x, y-1);
	
		}
		
}
			

	
	
	
	
	
	
	
	
	private void btnClicked(int flag)											//button click event callback function
	{
		Graphics g = this.stage.getGraphics();									//get Canvas GDI
		
		this.msg.setText("Counter : " + (++clickCnt));							//update Counter Label

		/*
		 * fillRect(int x, int y, int width, int height) 
		 */
		java.awt.Color DrawColor = Color.BLACK;
		
		switch(flag)															//Draw Rect
		{
		case BLUE:
			ChangeColor = 'B';
			DrawColor = Color.BLUE;
			break;
		case GREEN:
			ChangeColor = 'G';
			DrawColor = Color.GREEN;
			break;
		case CYAN:
			ChangeColor = 'C';
			DrawColor = Color.CYAN;
			break;
		case RED:
			ChangeColor = 'R';
			DrawColor = Color.RED;
			break;
		case MAGENTA:
			ChangeColor = 'M';
			DrawColor = Color.MAGENTA;
			break;
		case YELLOW:
			ChangeColor = 'Y';
			DrawColor = Color.YELLOW;
			break;
			
		
		}
		map[0][0] = ChangeColor;
		mapKari[0][0] = 1;
		
		ChangeMap(0, 0);
		
		//change display color
		
		
		
		for(int i = 0; i<SpaceMax; i++)
			for(int j = 0; j<SpaceMax; j++){
				
				if(mapKari[i][j] == 1){
					
					g.setColor(DrawColor);
					g.fillRect(j*50, i*50, this.blockSize, this.blockSize);
					
				}
					
				
				
			}


		
		CurrentColor = ChangeColor;
		
		/*System.out.println("Green!");			
		g.setColor(Color.GREEN);
		g.fillRect(0, 50, this.blockSize, this.blockSize);*/

		
	}
	
	public static void main(String[] args)									//main function
	{
		@SuppressWarnings("unused")
		floodit app = new floodit();

	}
}
