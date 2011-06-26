import java.awt.Color;
import java.awt.Container;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class SettingsConfigurator extends JFrame {
	private static final long serialVersionUID = 1L;
	private static String distances[];
	private static String zeroHandle[];
	private String inputFile;
	private String remapFile;
	private String distance;
	private String zero;
	JButton input = new JButton("No file selected");
	JButton remap = new JButton("No file selected");
	private JTextField error = new JTextField();
	public SettingsConfigurator() {
		JPanel p = new JPanel();
		
		input.addActionListener(new InputListener());
		remap.addActionListener(new RemapListener());

		error.setEditable(false);
		error.setForeground(Color.RED);
		
		JComboBox jc = new JComboBox(distances);
		jc.addActionListener(new DistanceHandlingChooser());
		jc.setSelectedIndex(1);

		JComboBox jc2 = new JComboBox(zeroHandle);
		jc2.addActionListener(new ZeroHandlingChooser());
		jc2.setSelectedIndex(1);

		JButton start = new JButton("execute");
		start.addActionListener(new SaveSettings());
		Container cp = getContentPane();
		GridLayout gl =  new GridLayout(5, 2);
		gl.setHgap(40);
		
		JTextField label1 = new JTextField();
		label1.setText("Select input file:");
		label1.setEditable(false);
		
		JTextField label2 = new JTextField();
		label2.setText("Select remap file:");
		label2.setEditable(false);
		
		JTextField label3 = new JTextField();
		label3.setText("Select distance:");
		label3.setEditable(false);
		
		JTextField label4 = new JTextField();
		label4.setText("Select zero handling:");
		label4.setEditable(false);
		
		p.setLayout(gl);
		p.add(label1);
		p.add(label2);
		p.add(input);
		p.add(remap);
		p.add(label3);
		p.add(label4);
		p.add(jc);
		p.add(jc2);
		p.add(start);
		p.add(error);
		cp.add(p);
	}

	class DistanceHandlingChooser implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			distance = ((JComboBox) e.getSource()).getSelectedItem().toString();
		}
	}

	class ZeroHandlingChooser implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			zero = ((JComboBox) e.getSource()).getSelectedItem().toString();
		}
	}

	class InputListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			JFileChooser c = new JFileChooser();

			int rVal = c.showOpenDialog(SettingsConfigurator.this);
			// jc.show();
			if (rVal == JFileChooser.APPROVE_OPTION) {
				inputFile = c.getSelectedFile().toString();
				remapFile = c.getSelectedFile().toString();
				input.setText(c.getSelectedFile().getName() + ": " + c.getSelectedFile().getAbsolutePath());
				remap.setText(c.getSelectedFile().getName() + ": " + c.getSelectedFile().getAbsolutePath());
				error.setVisible(false);
			}
			if (rVal == JFileChooser.CANCEL_OPTION) {
				// Do nothing
			}
		}
	}

	class RemapListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			JFileChooser c = new JFileChooser();
			// Demonstrate "Save" dialog:
			if (inputFile != null) {
				c.setSelectedFile(new File(inputFile));
			} else {
				error.setText("Please select input first.");
				error.setVisible(true);
				return;
			}
			int rVal = c.showOpenDialog(SettingsConfigurator.this);

			if (rVal == JFileChooser.APPROVE_OPTION) {
				remapFile = c.getSelectedFile().toString();
				remap.setText(c.getSelectedFile().getName() + ": " + c.getSelectedFile().getAbsolutePath());
				error.setVisible(false);
			}
			if (rVal == JFileChooser.CANCEL_OPTION) {
				// Do nothing
			}
		}
	}

	class SaveSettings implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			FileWriter settings;
			if (inputFile == null || remapFile == null) {
				error.setText("Please select a file");
				error.setVisible(true);
				return;
			}
			try {
				settings = new FileWriter("settings.conf");
				settings.write(inputFile + "\n");
				settings.write(remapFile + "\n");
				settings.write(distance + "\n");
				settings.write(zero + "\n");
				settings.close();
				System.exit(NORMAL);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				
				e1.printStackTrace();
			}

		}

	}

	public static void main(String[] args) {
		distances = args[0].split(",");
		zeroHandle = args[1].split(",");
		run(new SettingsConfigurator(), 500, 220);
	}

	public static void run(JFrame frame, int width, int height) {
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(width, height);
		frame.setVisible(true);
	}
} 