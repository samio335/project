package solver;

import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.lang.String;

import javax.swing.*;



@SuppressWarnings("serial")
class Stage extends JPanel
{
	final int SIZE = 12;
	final int blockSize = 50;		
	//final int width = 700, height = 700;
	final int width = SIZE*blockSize, height = SIZE*blockSize;
	
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
public class solver extends JFrame 
{
	final int BLUE = 0, GREEN = 1, CYAN = 2, RED = 3, MAGENTA = 4, YELLOW = 5;
	final int SIZE = 12;
	final int blockSize = 50;		//Cell Size
	final String inputFileNmae ="input10.txt";
	
	
	private Stage stage = new Stage();
	
	/* add more variable */
	private int clickCnt = 0;														//Counter variable
	
	private JLabel msg = new JLabel(" Counter : " + clickCnt + " / 25");				//private JLabel msg = new JLabel("Counter : " + clickCnt);						//Counter Message
	String [] BtnName = new String[] {"BLUE","GREEN","CYAN","RED","MAGENTA","YELLOW","Unknown"};
	private JLabel msgJiWng = new JLabel("Clicked : " + BtnName[6]);	
	
	
	// ************************************************
	// Append Member Variable 
	// ************************************************
	int SetColor [][] = new int [SIZE][SIZE];  		//Color Control	BLUE = 0, GREEN = 1, CYAN = 2, RED = 3, MAGENTA = 4, YELLOW = 5
	int ChangedCell [][] = new int[SIZE][SIZE];  	//Changed Control	0:NoChangeCel   1:ChangedCel
	int CurrentColor;							//Current ChangedColor
	int CheckRecFuncInfiniteLoop = 0;			//Recursive Function 무한루프 방지를 위해서
	int cntChangedCell = 0;						//버튼 클릭시 변환된(됄)셀의 수
	boolean canStartGame = false;
	

	// ************************************************************************************************
	//	생성자 , Main
	// ************************************************************************************************
	public solver() 
	{	
		/* window layout setting */
		//this.setLocation(300, 100);													//window position
		this.setLocation(300, 10);													//window position
		this.setTitle("Flood it " + SIZE + "x" + SIZE + " 6-color");									//window title
		
		this.setLayout(new GridBagLayout());										//Set layout Configuration
		GridBagConstraints cst = new GridBagConstraints();

		/* "Counter Label" */
		cst.fill = GridBagConstraints.HORIZONTAL;									//set "Counter Label" layout
		cst.gridx = 0;		cst.gridy = 0;										
		cst.gridwidth = 2;	
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
		cst.gridwidth = 1;
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
		
		//***********************************************************
		//	Append Button
		//***********************************************************
		/* AutoJiWng label */
		cst.fill = GridBagConstraints.HORIZONTAL;									//set "Counter Label" layout
		cst.gridx = 0;		
		cst.gridy = 4;							
		cst.gridwidth = 2;	
		//cst.gridheight = 2;	
		this.add(msgJiWng, cst);			
		
		
		
		/* AutoJiWngBtn Button */
		JButton AutoJiWngBtn = new JButton("AutoJiWng");									//make "Yellow" button object
		AutoJiWngBtn.addMouseListener(new MouseAdapter()								//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				AutoJiWngBtnClicked();
			}
		});
		AutoJiWngBtn.setBackground(Color.WHITE);										//set "Yellow" button layout
		cst.fill = GridBagConstraints.HORIZONTAL;
		cst.gridx = 0;
		cst.gridy = 5;
		this.add(AutoJiWngBtn,cst);
		
		
		/* Dummy Label */
		cst.fill = GridBagConstraints.HORIZONTAL;									//set "Counter Label" layout
		cst.gridx = 0;		
		cst.gridy = 6;	
		//cst.gridheight = 2;	
		this.add(new JLabel("    "), cst);		
																												
		/* InputBtn Button */
		JButton InputBtn = new JButton("Read InputFile");									//make "Yellow" button object
		InputBtn.addMouseListener(new MouseAdapter()								//register event callback function
		{
			public void mouseClicked(MouseEvent e)									//callback function implementation
			{
				InputbtnClicked();
			}
		});
		InputBtn.setBackground(Color.WHITE);										//set "Yellow" button layout
		cst.fill = GridBagConstraints.HORIZONTAL;
		cst.gridx = 0;
		cst.gridy = 7;
		this.add(InputBtn,cst);
		
		
		////////////////////////////////////////////////////////////
		
		//cst.gridx = 2;cst.gridy = 0;cst.gridheight = 6;cst.insets = new Insets(3, 3, 3, 3);	
		cst.gridx = 2;cst.gridy = 0;cst.gridheight = 8;cst.insets = new Insets(3, 3, 3, 3);
		this.add(this.stage,cst);


		this.pack();
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
		
		@SuppressWarnings("unused")
		Graphics g = this.stage.getGraphics();
		
	}
	
	
	//Main
	public static void main(String[] args)									//main function
	{
		@SuppressWarnings("unused")
		solver app = new solver();
	}
	
	
	// ************************************************************************************************
	//	event
	// ************************************************************************************************
	
	private void InputbtnClicked()
	{
		
		Graphics g = this.stage.getGraphics();									//get Canvas GDI
		
		///////////////////////////////////////////////////////////////////////////////////////////
		//input.txt file read
		///////////////////////////////////////////////////////////////////////////////////////////
		//FileStream Input.txt
		//
		//	Set SetColor Array
		//
		try {
			// -------------------------------------------------------------------
			// input.txt read
			// -------------------------------------------------------------------
			//File file = new File("input4.txt");
			File file = new File(inputFileNmae);
			BufferedReader filereader = new BufferedReader(new FileReader(file));
			String str;
			
			//final int BLUE = 0, GREEN = 1, CYAN = 2, RED = 3, MAGENTA = 4, YELLOW = 5;
			for (int line = 0; line < SIZE; line ++) 
			{
				str = filereader.readLine();
				if(str != null) {
					for (int cel = 0; cel < SIZE; cel++)
					{
						
						//System.out.println("Color =" + str.charAt(cel));
						//SetColor[line][cel] = str.charAt(cel);
						
						//BLUE = 0, GREEN = 1, CYAN = 2, RED = 3, MAGENTA = 4, YELLOW = 5;
						if ( str.charAt(cel) == 'B') {
							SetColor[line][cel] = 0;
							g.setColor(Color.BLUE);
							g.fillRect(cel*this.blockSize, line*this.blockSize, this.blockSize, this.blockSize);
						}
						else if ( str.charAt(cel) == 'G') {
							SetColor[line][cel] = 1;
							g.setColor(Color.GREEN);
							g.fillRect(cel*this.blockSize, line*this.blockSize, this.blockSize, this.blockSize);
						}
						else if ( str.charAt(cel) == 'C') {
							SetColor[line][cel] = 2;
							g.setColor(Color.CYAN);
							g.fillRect(cel*this.blockSize, line*this.blockSize, this.blockSize, this.blockSize);
						}
						else if ( str.charAt(cel) == 'R') {
							SetColor[line][cel] = 3;
							g.setColor(Color.RED);
							g.fillRect(cel*this.blockSize, line*this.blockSize, this.blockSize, this.blockSize);
						}
						else if ( str.charAt(cel) == 'M') {
							SetColor[line][cel] = 4;
							g.setColor(Color.MAGENTA);
							g.fillRect(cel*this.blockSize, line*this.blockSize, this.blockSize, this.blockSize);
						}
						else if ( str.charAt(cel) == 'Y') {
							SetColor[line][cel] = 5;
							g.setColor(Color.YELLOW);
							g.fillRect(cel*this.blockSize, line*this.blockSize, this.blockSize, this.blockSize);
						}
						else 
						{
							System.out.println("Data is incorrect.");
							filereader.close();
							return ;
						}
					}
				}
				else 
				{
					System.out.println("Data Format is incorrect.");
					filereader.close();
					return ;
				}
			}
		} catch(FileNotFoundException e){
			System.out.println(e);
		} catch(IOException e){
		  System.out.println(e);
		} catch (Exception e) {
			System.out.println(e.toString());
		}
		
		
		///////////////////////////////////////////////////////
		//initialize
		///////////////////////////////////////////////////////
		
		canStartGame = true;
		clickCnt = 0;		
		this.msg.setText("Counter: " + (clickCnt) + "/25");
		this.msgJiWng.setText("Clicked : " + BtnName[6]);
		
		//
		//	Set ChangedCell
		//
		for (int y=0; y < SIZE; y++)
			for (int x=0; x < SIZE; x++)
				ChangedCell[y][x] = 0;
		
		
		//
		//First Column
		//
		CurrentColor = SetColor[0][0];
		ChangedCell[0][0] = 1;
		
		CheckRecFuncInfiniteLoop = 0;
		RecSetColor(0,0);
		
	}
	
	
	private void btnClicked(int flag)											//button click event callback function
	{
		//게임 종료및 가능상태 체크
		if (clickCnt > 0 && !canStartGame) {
			JOptionPane.showMessageDialog(null, "game over!!");
			return;
		}
		else if (!canStartGame) {
			JOptionPane.showMessageDialog(null, "Cannot start game !");
			return;
		}
		
		//동일색 클릭시 무시
		if (flag == CurrentColor) 	//현재색을 제외한 나머지 색으로 시뮬레이션 한다.
			return;
		
		this.msg.setText("Counter: " + (++clickCnt) + "/25");	
		this.msgJiWng.setText("Clicked : " + BtnName[flag]);
		System.out.println("Counter: " + (clickCnt) + ",  Clicked : " + BtnName[flag]);
		
		//*************************************************
		//	Change Color of ChangedCell
		//*************************************************
		ChgColorOfChangedCell(flag, 'n');
		
		//*************************************************
		//	Check Complete
		//*************************************************
		int ChkComplete = 0;
		for (int y=0; y < SIZE; y++)
			for (int x=0; x < SIZE; x++)
				if (ChangedCell[y][x] == 1)
					ChkComplete++;
		
	
		if(clickCnt >= 25 && ChkComplete < (SIZE*SIZE) ) {
			canStartGame = false;
			JOptionPane.showMessageDialog(null, "game over!!");
		}
		else if (ChkComplete == (SIZE*SIZE) ) {
			canStartGame = false;
			JOptionPane.showMessageDialog(null, "The game is complete. count = "+ clickCnt);	
		}
		
		
	}
	

	private void AutoJiWngBtnClicked()
	{
		//게임 종료및 가능상태 체크
		if (clickCnt > 0 && !canStartGame) {
			JOptionPane.showMessageDialog(null, "game over!!");
			return;
		}
		else if (!canStartGame) {
			JOptionPane.showMessageDialog(null, "Cannot start game !");
			return;
		}
		
		//백업
		int SetColorBackup [][] = new int [SIZE][SIZE];  		//Backup
		int ChangedCellBackup [][] = new int[SIZE][SIZE];  	//Backup
		int CurrentColorBackup = 6;
		
		//최적버튼
		int SeletedBestFlag = 6;
		
		System.out.println("***** Simulation Start ****************************************");
		System.out.println("** Current Color : "+  BtnName[CurrentColor]);
		
		//BLUE = 0, GREEN = 1, CYAN = 2, RED = 3, MAGENTA = 4, YELLOW = 5
		//UNKNOWN = 6
		
		
		//*************************************************
		//	Search Best Flag Method : 가장 많은 주변색을 변환시키는 버튼을 최적으로 판단한다
		//		현재 상태를 그래도 백업해 두고
		//		현재 색 이외의 모든색으로 한번 색 실행해 본다
		//		변화된 셀의 갯수를 세어서 가장 많은 변화된 색으로 결정한다.
		//		동률의 색이 있을시 목적지로 향하는 중심선에 가까운 색으로 결정한다
		//			중심선에 가까운 색이란 x-y의 절대값이 적은것
		//*************************************************

		//현재 상태 백업
		CurrentColorBackup = CurrentColor;
		for (int y=0; y < SIZE; y++)
			for (int x=0; x < SIZE; x++) {
				SetColorBackup[y][x] = SetColor[y][x];
				ChangedCellBackup[y][x] = ChangedCell[y][x];
			}
		
		
		//현재 색 이외의 색에 주변에 얼마나 있는지를 실행해 본다.
		int cntChangedCellPerColor[] = {0,0,0,0,0,0};
		int CountMax = 0; 
		for (int iFlag=0; iFlag < 6; iFlag++) {
			if (iFlag == CurrentColorBackup) 	//현재색을 제외한 나머지 색으로 시뮬레이션 한다.
				continue;
			
			//백업해둔 현재상태로 원위치 시킨다
			CurrentColor = CurrentColorBackup;
			for (int y=0; y < SIZE; y++)
				for (int x=0; x <SIZE ; x++) {
					SetColor[y][x] = SetColorBackup[y][x];
					ChangedCell[y][x] = ChangedCellBackup[y][x];
				}
			
			//*************************************************
			//	Change Color of ChangedCell : Simulation Mode
			//*************************************************
			ChgColorOfChangedCell(iFlag, 'y');
			cntChangedCellPerColor[iFlag] = cntChangedCell;
			//System.out.println("cntChangedCell[" + BtnName[iFlag] + "]: " + cntChangedCellPerColor[iFlag]);
			
			
			//변환된 갯수 최대값 저장
			if (cntChangedCell >= CountMax ) 
				CountMax = cntChangedCell;
		}
		
		System.out.println("** ChangedCell CountMax : " + CountMax);
	
		//대각선에 제일 가까운 셀을 선정해서, 대갓선에 가까운 정도를 계산한다
		//	대각선에 가까운 최대 외측셀 : x + y 가 제일 큰 셀
		//	대각선에 가까운 정도 : 절대값(x-y)
		int CheckDiagonalPerColor[] = {999,999,999,999,999,999};	//절대값(x-y)가 작은것이 대각선에 가깝다고 판단한다.
		int TempX = 0, TempY = 0, TempXYPlus = 0;
		int DiagonalBestValue = 999;
		for (int iFlag=0; iFlag < 6; iFlag++) {
			if (iFlag == CurrentColorBackup) 	//현재색 제외
				continue;
			
			System.out.print("cntChangedCellPerColor[" +  BtnName[iFlag] + "] : " + cntChangedCellPerColor[iFlag]);
			
			if (cntChangedCellPerColor[iFlag] < CountMax) {	//변화된 셀이 최대값의 색이 대상이 아님
				System.out.println(", not Best");
				continue;
			}
				
			
			//대각선에 가까운 최대 외측셀 선정
			TempX = 0;	TempY = 0;	TempXYPlus = 0;
			for (int y=0; y < SIZE; y++)
				for (int x=0; x < SIZE; x++) 
					if (ChangedCell[y][x] == 1) 
						if ( x + y >= TempXYPlus) {
							TempXYPlus = x + y;
							TempX = x;
							TempY = y;
							//System.out.println("(x,y) = (" + x + "," + y + "), TempXYPlus=" + TempXYPlus);
						}
			
			//대각선에 가까운 최대 외측셀의 대각선에 가까운 정도 계산
			CheckDiagonalPerColor[iFlag] = Math.abs(TempX-TempY);
			
			//대각선에 가가운 정도의 베스트 값을 저장
			if (CheckDiagonalPerColor[iFlag] <= DiagonalBestValue )
				DiagonalBestValue = CheckDiagonalPerColor[iFlag];
			
			System.out.println(", DiagonalCell(" + TempX + "," +  TempY + "),  DiagonalValue : " + CheckDiagonalPerColor[iFlag]);
		}
		System.out.println("** Best Diagonal Value : " + DiagonalBestValue);

		//시뮬레이션 결과 확인 2 : 대각선 정도 확인, 대각선도 동률이면 iFlag큰값으로 선정됌
		for (int iFlag=0; iFlag < 6; iFlag++) {
			if (iFlag == CurrentColorBackup) 	//현재색 제외
				continue;
			
			if (cntChangedCellPerColor[iFlag] == CountMax && CheckDiagonalPerColor[iFlag] == DiagonalBestValue ) {
				
				SeletedBestFlag = iFlag;
			}
				
		}
		System.out.println("** Best Selected Flag : " + BtnName[SeletedBestFlag]);
		System.out.println("***** Simulation End ****************************************");
		
		
		//*************************************************
		//	Click SeletedBestFlag
		//*************************************************
		//원상태로 돌려놓고
		CurrentColor = CurrentColorBackup;
		for (int y=0; y < SIZE; y++)
			for (int x=0; x < SIZE; x++) {
				SetColor[y][x] = SetColorBackup[y][x];
				ChangedCell[y][x] = ChangedCellBackup[y][x];
			}
		
		//SeletedBestFlag 실행
		btnClicked(SeletedBestFlag);
	}
	
	
	// ************************************************************************************************
	//	internal function
	// ************************************************************************************************
	//flag 0-5
	//isSimulation y, n 
	private void ChgColorOfChangedCell(int flag, char isSimulation) {
		
		cntChangedCell = 0;
		
		//ChangedCell[yy][xx] 
		for (int y=0; y < SIZE; y++)
			for (int x=0; x < SIZE; x++)
				//*************************************************
				// 현재 변환된 영억으로 표시된 곳만 처리대상으로 한다
				//*************************************************
				if (ChangedCell[y][x] == 1) {
					//*************************************************
					// Change SetColor Array
					//*************************************************
					SetColor[y][x] = flag;	
					
					//*************************************************
					// Change Display
					//*************************************************
					if (isSimulation != 'y') {
						Graphics g = this.stage.getGraphics();									//get Canvas GDI
						switch(flag)															//Draw Rect
						{
						case BLUE:
							//System.out.println("Blue!");
							g.setColor(Color.BLUE);
							g.fillRect(x*this.blockSize, y*this.blockSize, this.blockSize, this.blockSize);
							break;
						case GREEN:
							//System.out.println("Green!");			
							g.setColor(Color.GREEN);
							g.fillRect(x*this.blockSize, y*this.blockSize, this.blockSize, this.blockSize);
							break;
						case CYAN:
							//System.out.println("Cyan!");		
							g.setColor(Color.CYAN);
							g.fillRect(x*this.blockSize, y*this.blockSize, this.blockSize, this.blockSize);
							break;
						case RED:
							//System.out.println("Red!");		
							g.setColor(Color.RED);
							g.fillRect(x*this.blockSize, y*this.blockSize, this.blockSize, this.blockSize);
							break;
						case MAGENTA:
							//System.out.println("Magenta!");			
							g.setColor(Color.MAGENTA);
							g.fillRect(x*this.blockSize, y*this.blockSize, this.blockSize, this.blockSize);
							break;
						case YELLOW:
							//System.out.println("Yellow!");			
							g.setColor(Color.YELLOW);
							g.fillRect(x*this.blockSize, y*this.blockSize, this.blockSize, this.blockSize);
							break;
						}
					}
				
					//*************************************************
					//	주변의 같은색이 있으면 변환된 영역으로 바꾼다
					//*************************************************
					CurrentColor = flag;
					CheckRecFuncInfiniteLoop = 0;
					RecSetColor(y,x);
					
				}
		
	}
	
	
	private void RecSetColor(int y, int x) {
		
		//무한루프 체크
		CheckRecFuncInfiniteLoop++;
		//System.out.println("CheckRecFunc Call Count = " + CheckRecFuncInfiniteLoop + ", y=" + y + ", x=" + x);
		if (CheckRecFuncInfiniteLoop > 784) {
			System.out.println("CheckRecFunc Call Count(********infinite loop*****) = " + CheckRecFuncInfiniteLoop);
			return;
		}
		
		
		int  yy; int xx;
		
		//주변색변화여부 체크
		int upper = 1;
		int down = 1;
		int left = 1;
		int right = 1;
		
		//upper
		yy = y - 1;
		xx = x;
		if ( yy >= 0 && xx >= 0 && yy < SIZE && xx < SIZE && ChangedCell[yy][xx] == 0 && CurrentColor == SetColor[yy][xx]){
			ChangedCell[yy][xx] = 1;
			cntChangedCell++;
			RecSetColor(yy,xx);
		}
		else
			upper = 0;

		//down
		yy = y + 1;
		xx = x;
		if ( yy >= 0 && xx >= 0 && yy < SIZE && xx < SIZE && ChangedCell[yy][xx] == 0 && CurrentColor == SetColor[yy][xx]){
			ChangedCell[yy][xx] = 1;
			cntChangedCell++;
			RecSetColor(yy,xx);
		}
		else
			down = 0;
		
		//left
		yy = y;
		xx = x-1;
		if ( yy >= 0 && xx >= 0 && yy < SIZE && xx < SIZE && ChangedCell[yy][xx] == 0 && CurrentColor == SetColor[yy][xx]){
			ChangedCell[yy][xx] = 1;
			cntChangedCell++;
			RecSetColor(yy,xx);
		}
		else
			left = 0;
		
		//right
		yy = y;
		xx = x+1;
		if ( yy >= 0 && xx >= 0 && yy < SIZE && xx < SIZE && ChangedCell[yy][xx] == 0 && CurrentColor == SetColor[yy][xx]){
			ChangedCell[yy][xx] = 1;
			cntChangedCell++;
			RecSetColor(yy,xx);
		}
		else 
			right = 0;
		
		
		if (upper == 0 && down == 0 && left == 0 && right == 0 )
			return;
		
	}
	

}