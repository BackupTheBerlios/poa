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
import gcover.util.Formatter;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Date;

import org.apache.ecs.Entities;
import org.apache.ecs.*;
import org.apache.ecs.Doctype.XHtml10Strict;
import org.apache.ecs.xhtml.*;

/**
 * XHTMLOutputter
 * 
 * @author Tammo van Lessen
 * @version $Id: XHTMLOutputter.java,v 1.3 2004/01/07 14:37:12 vanto Exp $
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
		XhtmlDocument doc = new XhtmlDocument();
		doc.setDoctype(new XHtml10Strict());
		doc.appendHead(new link().setType("text/css").setRel("Stylesheet").setHref("style.css"));

		body content = new body();
		doc.appendBody(content);
		for (int i=0; i<files.length; i++) {
			output(files[i]);
			content.setBgColor("white");
			content.addElement(new a(files[i].getName()+".html",files[i].getName()));
			content.addElement(new StringElement("&nbsp;("+Formatter.formatNumber(files[i].getCoverage()*100,2)+"%)"));
			content.addElement(new br());
		}
		try {
			FileWriter fw = new FileWriter(dir.getAbsolutePath()+File.separatorChar+"files.html");
			//doc.output(fw);
			doc.getHtml().setPrettyPrint(true);
			fw.write(doc.toString());
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
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
								.addElement(new td(new b("Exec.")).setAlign("right"))
								.addElement(new td(""+file.getExecutionCount()).setAlign("right")) )
							.addElement(new tr()
								.addElement(new td())
								.addElement(new td(new b("IL")).setAlign("right"))
								.addElement(new td(""+file.getInstrumentedLinesCount()).setAlign("right"))
								.addElement(new td(new b("EL")).setAlign("right"))
								.addElement(new td(""+file.getExecutedLinesCount()).setAlign("right"))
							)

							.setAlign("right"))
						.setClass("headerStats")
						
					)			
			);
					
			int cov = (int)(file.getCoverage()*200);
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
						.addElement(new td(new b(Formatter.formatNumber(file.getCoverage()*100,2)+"%")).setAlign("center").setClass("graphBarLeft"))
						.addElement(new td("BAR")
							.addElement(new table()
								.addElement(new tbody()
									.addElement(new tr()
										.addElement(new td()
											.addElement(new img()
												.setSrc("trans.gif")
												.setAlt("coverage")
												.setHeight(12)
												.setWidth(cov)
											)
											.setClass("covered")
										)
										.addElement(new td()
											.addElement(new img()
												.setSrc("trans.gif")
												.setAlt("coverage")
												.setHeight(12)
												.setWidth(200-cov)
											)
											.setClass("uncovered")
										)
									)
								)
								.setCellSpacing(0)
								.setClass("barGraph")
							
							)
							.setClass("graphBar")
						
						)
					)

			);
			doc.appendBody(header);
			table sourceView = new table();
			sourceView.setCellPadding(0).setCellSpacing(0).setClass("srcView");
			tbody srcViewBody = new tbody();
			sourceView.addElement(srcViewBody);

			for (int i=0; i<file.getLines().length; i++) {
				LineInfo li = file.getLines()[i];
				if (li.isInstrumented()) {
					if (li.getExecCount() == 0) {
						srcViewBody
							.addElement(new tr()
								.addElement(new td(""+(i+1)).setAlign("right").setClass("lineCountHilight"))
								.addElement(new td(""+li.getExecCount()).setAlign("right").setClass("coverageCountHilight"))
								.addElement(new td(new span()
													.addElement(new pre(li.getSourceLine()).setClass("srcLine")).setClass("srcHilight")).setClass("srcLine"))
							);
					} else {
						srcViewBody
							.addElement(new tr()
								.addElement(new td(""+(i+1)).setAlign("right").setClass("lineCountHilight"))
								.addElement(new td(""+li.getExecCount()).setAlign("right").setClass("lineCountHilight"))
								.addElement(new td(new pre(li.getSourceLine()).setClass("srcLine")).setClass("srcLine"))
							);
					}
				} else {
					srcViewBody
						.addElement(new tr()
							.addElement(new td(""+(i+1)).setAlign("right").setClass("lineCount"))
							.addElement(new td("").setAlign("right").setClass("coverageCount"))
							.addElement(new td(new pre(li.getSourceLine()).setClass("srcLine")).setClass("srcLine"))
						);
				}
			}
			
			doc.appendBody(sourceView);
			
			//doc.output(fw);
			doc.getHtml().setPrettyPrint(true);
			fw.write(doc.toString());
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}

}
