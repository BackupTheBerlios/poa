/*
 * gcover
 * TODO
 * Created on 15.07.2003
 *
 */
package gcover.output;

import gcover.FileInfo;
import gcover.Project;
import gcover.util.Formatter;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Date;

import org.apache.ecs.Entities;
import org.apache.ecs.XhtmlDocument;
import org.apache.ecs.Doctype.XHtml10Strict;
import org.apache.ecs.xhtml.*;

/**
 * XHTMLOutputter
 * 
 * @author Tammo van Lessen
 * @version $Id: XHTMLOutputter.java,v 1.1 2003/07/15 18:06:32 vanto Exp $
 */
public class XHTMLOutputter implements Outputter {

	private File dir;
	
	public XHTMLOutputter(String basedir) {
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
			XhtmlDocument doc = new XhtmlDocument();
			doc.setDoctype(new XHtml10Strict());
			doc.appendTitle("GCover - Code Coverage for "+file.getName());
			doc.appendHead(new link().setType("text/css").setRel("Stylesheet").setHref("style.css"));
			table reportHeader = new table().setWidth("100%");
			reportHeader.addElement(
				new tr()
					.addElement(new td("GCover - Coverage Report")
						.setVAlign("top")
							.addElement(new div()
								.addElement(new b("Coverage timestamp:"))
								.addElement(Entities.NBSP)
								.addElement(new Date().toString())
									.setClass("timestamp"))
						.setClass("titleText"))
					.addElement(new td().setVAlign("top").setAlign("right").setWidth("40%")
						.addElement(new table()
							.addElement(new tr()
								.addElement(new td("file stats").setNoWrap(true))
								.addElement(new td(new b("LOC")).setAlign("right"))
								.addElement(new td(""+file.getLines().length).setAlign("right"))
								.addElement(new td(new b("EL")).setAlign("right"))
								.addElement(new td(""+file.getExecutedLinesCount()).setAlign("right")) )
							.addElement(new tr()
								.addElement(new td())
								.addElement(new td(new b("IL")).setAlign("right"))
								.addElement(new td(""+file.getInstrumentedLinesCount()).setAlign("right"))
								.addElement(new td(new b("Exec.")).setAlign("right"))
								.addElement(new td(""+file.getExecutionCount()).setAlign("right"))
							)

							.setAlign("right"))
						.setClass("headerStats")
						
					)			
			);
					
			
			table header = new table();
			header.setCellSpacing(0).setCellPadding(2);
			header.addElement(
				new tbody()
					.addElement(new tr()
						.addElement(new td().setColSpan(6).addElement(reportHeader).setClass("reportHeader")))
					.addElement(new tr()
						.addElement(new td(Entities.NBSP).setColSpan(6)))
					.addElement(new tr()
						.addElement(new td(new b(Entities.NBSP+"Source file")).setClass("graphHeaderLeft"))
						.addElement(new td(Entities.NBSP).setClass("graphHeader"))
						.addElement(new td(Entities.NBSP).setClass("graphHeader"))
						.addElement(new td(Entities.NBSP).setClass("graphHeader"))
						.addElement(new td(new b("TOTAL")).setColSpan(2).setAlign("left").setClass("graphHeader"))
					)
					.addElement(new tr()
						.addElement(new td(new b(file.getName())).setClass("graphItem"))
						.addElement(new td(Entities.NBSP).setAlign("center").setClass("graphPercent"))
						.addElement(new td(Entities.NBSP).setAlign("center").setClass("graphPercent"))
						.addElement(new td(Entities.NBSP).setAlign("center").setClass("graphPercent"))
						.addElement(new td(new b(Formatter.formatNumber(file.getCoverage()*100,2))).setAlign("center").setClass("graphBarLeft"))
						.addElement(new td("BAR").setClass("graphBar"))
					)

			);
			doc.appendBody(header);
			//doc.output(fw);
			doc.getHtml().setPrettyPrint(true);
			fw.write(doc.toString());
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}
