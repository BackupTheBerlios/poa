/*
 * gcover
 * TODO
 * Created on 15.07.2003
 *
 */
package gcover.output;

import gcover.FileInfo;
import gcover.LineInfo;
import gcover.Project;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import org.apache.ecs.html.BR;
import org.apache.ecs.html.H1;
import org.apache.ecs.html.Head;
import org.apache.ecs.html.Html;
import org.apache.ecs.html.IMG;
import org.apache.ecs.html.Link;
import org.apache.ecs.html.PRE;
import org.apache.ecs.html.Span;
import org.apache.ecs.html.TD;
import org.apache.ecs.html.TR;
import org.apache.ecs.html.Table;
import org.apache.ecs.html.Title;

/**
 * HTMLOutputter
 * 
 * @author Tammo van Lessen
 * @version $Id: HTMLOutputter.java,v 1.1 2003/07/15 18:06:32 vanto Exp $
 */
public class HTMLOutputter implements Outputter {

	private File dir;
	
	public HTMLOutputter(String basedir) {
		dir = new File(basedir);
		if (!dir.exists()) {
			dir.mkdir();
		}
	}
	/**
	 * @see gcover.output.Outputter#output(gcover.Project)
	 */
	public void output(Project prj) {
		FileInfo[] files = prj.getFiles();
		for (int i=0; i<files.length; i++) {
			output(files[i]);
		}
	}
	
	public void output(FileInfo file) {
		try {
			FileWriter fw = new FileWriter(dir.getAbsolutePath()+File.separatorChar+file.getName()+".html");
			Table table = new Table();
			for (int i=0; i<file.getLines().length; i++) {
				LineInfo li = file.getLines()[i];
				if (li.isInstrumented()) {
					if (li.getExecCount() == 0) {
						table
							.addElement(new TR()
								.addElement(new TD(""+(i+1)).setAlign("right").setClass("lineCountHilight"))
								.addElement(new TD(""+li.getExecCount()).setAlign("right").setClass("coverageCountHilight"))
						.addElement(new TD(new Span()
														.addElement(new PRE(li.getSourceLine()).setClass("srcLine")).setClass("srcHilight")).setClass("srcLine"))
							).setClass("srcView");
					} else {
						table
							.addElement(new TR()
								.addElement(new TD(""+(i+1)).setAlign("right").setClass("lineCountHilight"))
								.addElement(new TD(""+li.getExecCount()).setAlign("right").setClass("lineCountHilight"))
								.addElement(new TD(new PRE(li.getSourceLine()).setClass("srcLine")).setClass("srcLine"))
							).setClass("srcView");
					}
				} else {
					table
						.addElement(new TR()
							.addElement(new TD(""+(i+1)).setAlign("right").setClass("lineCount"))
							.addElement(new TD("").setAlign("right").setClass("coverageCount"))
							.addElement(new TD(new PRE(li.getSourceLine()).setClass("srcLine")).setClass("srcLine"))
						).setClass("srcView");
				}
			}
			int cov = (int)file.getCoverage()*200;
			Table bar = new Table().setCellSpacing(0);
			bar.setClass("barGraph");
			bar.addElement(new TR()
				.addElement(new TD(new IMG().setSrc("trans.gif").setWidth(cov).setHeight(12)).setClass("covered"))
			.addElement(new TD(new IMG().setSrc("trans.gif").setWidth(200-cov).setHeight(12)).setClass("uncovered")));
			
			Html html = new Html()
				.addElement(new Head()
					.addElement(new Link().setRel("stylesheet").setType("text/css").setHref("style.css").setTitle("Style"))
					.addElement(new Title("Coverage for File "+file.getName()))
				.addElement(new H1(file.getName()))
				.addElement(new BR().addElement(bar))
				.addElement(table));
			fw.write(html.toString());
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
