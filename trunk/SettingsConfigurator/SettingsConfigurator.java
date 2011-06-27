import java.awt.Color;
import java.awt.Container;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class SettingsConfigurator extends JFrame {
	private static final long serialVersionUID = 1L;
	private static String distances[];
	private static String zeroHandle[];
    private static String run_id;
	private String inputFile;
	private String remapFile;
	private String classifiedFile;
	private String distance;
	private String zero;
	private JButton input = new JButton("No file selected");
	private JButton remap = new JButton("No file selected");
	private JButton classified = new JButton("No file selected");
	private boolean supervised = false;
	private JTextField error = new JTextField();
	private JTextField class_file = new JTextField("Select file:");
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
		GridLayout gl =  new GridLayout(7, 2);
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
		
		JCheckBox cb = new JCheckBox("Supervised?", false);
		cb.addActionListener(new SupervisedListener());
		classified.setVisible(false);
		class_file.setVisible(false);
		p.setLayout(gl);
		p.add(label1);
		p.add(label2);
		p.add(input);
		p.add(remap);
		p.add(label3);
		p.add(label4);
		p.add(jc);
		p.add(jc2);
		p.add(cb);
		JTextField tf = new JTextField();
		tf.setVisible(false);
		class_file.setEditable(false);
		class_file.setHorizontalAlignment(JTextField.RIGHT);
		classified.addActionListener(new SupervisedFileSelect());
		p.add(tf);
		p.add(class_file);
		p.add(classified);
		p.add(start);
		p.add(error);
		cp.add(p);
	}

	class DistanceHandlingChooser implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			distance = ((JComboBox) e.getSource()).getSelectedItem().toString();
		}
	}

	class ZeroHandlingChooser implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			zero = ((JComboBox) e.getSource()).getSelectedItem().toString();
		}
	}

	class InputListener implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			JFileChooser c = new JFileChooser(new File("example_data"));

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
			JFileChooser c = new JFileChooser(new File("example_data"));
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

		public void actionPerformed(ActionEvent e) {
			FileWriter settings;
			if (inputFile == null || remapFile == null) {
				error.setText("Please select a file");
				error.setVisible(true);
				return;
			}
			if (supervised == true && classifiedFile == null) {
				error.setText("Please select a file with classification");
				error.setVisible(true);
				return;
			}
			try {
				settings = new FileWriter("SettingsConfigurator\\settings.conf");
                settings.write(run_id + "\n");
				settings.write(inputFile + "\n");
				settings.write(remapFile + "\n");
				settings.write(distance + "\n");
				settings.write(zero + "\n");
				if (supervised) {
					settings.write(classifiedFile + "\n");
				}
				settings.close();
				System.exit(NORMAL);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				
				e1.printStackTrace();
			}

		}

	}

	class SupervisedListener implements  ActionListener {
		public void actionPerformed(ActionEvent e) {
			// TODO Auto-generated method stub
			if (((JCheckBox) e.getSource()).isSelected()) {
				supervised = true;
				classified.setVisible(true);
				class_file.setVisible(true);
			} else {
				supervised = false;
				classifiedFile = null;
				classified.setVisible(false);
				class_file.setVisible(false);
			}
		}
		
	}
	
	class SupervisedFileSelect implements  ActionListener {
		public void actionPerformed(ActionEvent e) {
			// TODO Auto-generated method stub
			JFileChooser c = new JFileChooser();
			int rVal = c.showOpenDialog(SettingsConfigurator.this);

			if (rVal == JFileChooser.APPROVE_OPTION) {
				classifiedFile = c.getSelectedFile().toString();
				classified.setText(c.getSelectedFile().getName() + ": " + c.getSelectedFile().getAbsolutePath());
			}
			if (rVal == JFileChooser.CANCEL_OPTION) {
				// Do nothing
			}
		}
		
	}
	public static void main(String[] args) {
        run_id = args[0];
		distances = args[1].split(",");
		zeroHandle = args[2].split(",");
		run(new SettingsConfigurator(), 500, 220);
	}

	public static void run(JFrame frame, int width, int height) {
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(width, height);
		frame.setVisible(true);
	}
} 