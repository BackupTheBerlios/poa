/*
 *  GCover
 *
 *  Copyright by Tammo van Lessen, Steffen Pingel
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  Version 2 as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
package gcover.output;

import gcover.FileInfo;
import gcover.LineInfo;
import gcover.Project;
import gcover.util.*;

import java.io.File;
import java.io.FileWriter;
import java.io.*;
import java.util.Date;

import org.apache.ecs.Entities;
import org.apache.ecs.*;
import org.apache.ecs.Doctype.XHtml10Strict;
import org.apache.ecs.xhtml.*;

/**
 * XHTMLOutputter
 * 
 * @author Tammo van Lessen
 * @version $Id: XHTMLOutputter.java,v 1.8 2004/01/11 12:09:30 squig Exp $
 */
public class XHTMLOutputter implements Outputter {

	private File dir;
	
	public XHTMLOutputter(String basedir) {
		dir = new File(basedir);
		if (!dir.exists()) {
			dir.mkdirs();
		}
	}

	private XhtmlDocument createDocument(String title)
	{
		XhtmlDocument doc = new XhtmlDocument();
		doc.setDoctype(new XHtml10Strict());
		doc.appendTitle(title);
		doc.appendHead(new link().setType("text/css").setRel("Stylesheet").setHref("style.css"));
		return doc;
	}

	private void writeDocument(XhtmlDocument doc, String filename)
	{
		try {
			FileWriter fw = new FileWriter(dir.getAbsolutePath()+File.separatorChar+filename);
			//doc.output(fw);
			doc.getHtml().setPrettyPrint(true);
			fw.write(doc.toString());
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void copyResources()
	{
		try {
			Util.copy(getClass().getResourceAsStream("style.css"),
					  new FileOutputStream(new File(dir, "style.css")));
			Util.copy(getClass().getResourceAsStream("index.html"),
					  new FileOutputStream(new File(dir, "index.html")));
		} 
		catch (IOException e) {
			System.err.println("Could not copy resource");
			e.printStackTrace(System.err);
		}
	}
			

	/**
	 * @see gcover.output.Outputter#output(gcover.Project)
	 */
	public void output(Project prj) {
		FileInfo[] files = prj.getFiles();

		// prepare list of files
		body content = new body();

		for (int i=0; i < files.length; i++) {
			// write file
			output(files[i]);

			// append file to list of files
			content.setBgColor("white");
			content.addElement(new a(files[i].getName()+".html", files[i].getName()).setTarget("gcoverClassFrame"));
			content.addElement(new StringElement("&nbsp;("+Formatter.formatNumber(files[i].getCoverage()*100,2)+"%)"));
			content.addElement(new br());
		}

		// write list of files
		XhtmlDocument doc = createDocument("Files");
		doc.appendBody(content);
		writeDocument(doc, "files.html");

		// FIX: write overview.html
		doc = createDocument("Overview");
		writeDocument(doc, "overview.html");
	}
	
	public void output(FileInfo file) {
// 		try {
		System.out.println("Generating " + file.getName() + ".html");

			XhtmlDocument doc = createDocument("GCover - Code Coverage for "+file.getName());
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

			writeDocument(doc, file.getName()+".html");
// 		} catch (IOException e) {
// 			e.printStackTrace();
// 		}
		
	}

}
