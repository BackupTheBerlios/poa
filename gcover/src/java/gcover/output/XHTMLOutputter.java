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
import gcover.util.Formatter;
import gcover.util.Util;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Date;

import org.apache.ecs.Element;
import org.apache.ecs.Entities;
import org.apache.ecs.StringElement;
import org.apache.ecs.XhtmlDocument;
import org.apache.ecs.Doctype.XHtml10Strict;
import org.apache.ecs.xhtml.a;
import org.apache.ecs.xhtml.b;
import org.apache.ecs.xhtml.body;
import org.apache.ecs.xhtml.br;
import org.apache.ecs.xhtml.div;
import org.apache.ecs.xhtml.h1;
import org.apache.ecs.xhtml.img;
import org.apache.ecs.xhtml.link;
import org.apache.ecs.xhtml.pre;
import org.apache.ecs.xhtml.span;
import org.apache.ecs.xhtml.table;
import org.apache.ecs.xhtml.tbody;
import org.apache.ecs.xhtml.td;
import org.apache.ecs.xhtml.th;
import org.apache.ecs.xhtml.tr;
import org.w3c.dom.html.HTMLElement;

/**
 * XHTMLOutputter
 * 
 * @author Tammo van Lessen
 * @version $Id: XHTMLOutputter.java,v 1.13 2004/01/14 20:18:54 squig Exp $
 */
public class XHTMLOutputter implements Outputter {

	public static final int barLength = 100;
	
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
		copyResource("style.css");
		copyResource("index.html");
		copyResource("green.png");
		copyResource("red.png");
	}			

	public void copyResource(String filename)
	{
		InputStream in = getClass().getResourceAsStream(filename);
		if (in != null) {
			try {
				Util.copy(in, new FileOutputStream(new File(dir, filename)));
			} 
			catch (IOException e) {
				System.err.println("Could not copy resource: " + filename);
			}
		}
		else {
			System.err.println("Could not copy missing resource: " + filename);
		}
	}

	/**
	 * @see gcover.output.Outputter#output(gcover.Project)
	 */
	public void output(Project prj) {
		FileInfo[] files = prj.getFiles();

		// prepare list of files
		body content = new body();
		content.addElement(new h1("All Files"));
		
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

		// write overview.html
		outputOverview(prj, files);
	}
	
	public void output(FileInfo file) {
		System.out.println("Generating " + file.getName() + ".html");

		XhtmlDocument doc = createDocument("GCover - Code Coverage for "+file.getName());

		// header
		table headerView = new table();
		headerView.setCellSpacing(0).setCellPadding(2).setWidth("100%").setClass("headerView");
		headerView
			.addElement(new tr()
				.addElement(new td(new h1("GCover - Coverage Report"))
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
							.addElement(new td(new b("LOC")).setAlign("right"))
							.addElement(new td(""+file.getLinesOfCode()).setAlign("left")))
						.addElement(new tr()
							.addElement(new td(new b("Instrumented")).setAlign("right"))
							.addElement(new td(""+file.getInstrumentedLinesCount()).setAlign("left")))
						.addElement(new tr()
							.addElement(new td(new b("Executed")).setAlign("right"))
							.addElement(new td(""+file.getExecutedLinesCount()).setAlign("left"))))))
			.addElement(new tr()
				.addElement(new td("Source file: ").setVAlign("top")
					.addElement(new b(file.getName())))
				.addElement(new td()))
			.addElement(new tr()
				.addElement(new td("Total:").setVAlign("top")
					.addElement(Entities.NBSP)
					.addElement(new b(formatRate(file.getCoverage())))
					.addElement(Entities.NBSP)
					.addElement(createBar(file.getCoverage())))
				.addElement(new td()));
		doc.appendBody(new div().addElement(headerView).setClass("headerView"));
		doc.appendBody(new br());

		// source code
		table sourceView = new table();
		sourceView.setCellPadding(0).setCellSpacing(0).setClass("sourceView");
		tbody srcViewBody = new tbody();
		sourceView.addElement(srcViewBody);

		LineInfo[] lines = file.getLines();
		for (int i=0; i<lines.length; i++) {
			LineInfo li = lines[i];

			String numberStyle 
				= (li.isInstrumented()) 
				? (li.getExecCount() == 0)
				? "uncovered"
				: "covered"
				: "notinstrumented";

			td srcLine = new td(new span()
				.addElement(new pre(JavaToHtml.syntaxHighlight(li.getSourceLine()))));
			if (li.isInstrumented() && li.getExecCount() == 0) {
				srcLine.setClass("uncovered");
			}
			
			srcViewBody
				.addElement(new tr()
					.addElement(new td(""+(i+1)).setAlign("right").setClass(numberStyle))
					.addElement(new td((li.isInstrumented()) ? ""+li.getExecCount() : "")
						.setAlign("right").setClass(numberStyle))
					.addElement(srcLine));
		}
		doc.appendBody(sourceView);

		writeDocument(doc, file.getName()+".html");
	}

	public void outputOverview(Project project, FileInfo[] files) {
		System.out.println("Generating overview.html");

		XhtmlDocument doc = createDocument("GCover - Overview");

		// header
		table headerView = new table();
		headerView.setCellSpacing(0).setCellPadding(2).setWidth("100%").setClass("headerView");
		headerView
			.addElement(new tr()
				.addElement(new td(new h1("GCover - Coverage Report"))
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
							.addElement(new td(new b("Files")).setAlign("right"))
							.addElement(new td(""+project.getFileCount()).setAlign("left")))
						.addElement(new tr()
							.addElement(new td(new b("LOC")).setAlign("right"))
							.addElement(new td(""+project.getLinesOfCode()).setAlign("left")))
						.addElement(new tr()
							.addElement(new td(new b("Instrumented")).setAlign("right"))
							.addElement(new td(""+project.getInstrumentedLinesCount()).setAlign("left")))
						.addElement(new tr()
							.addElement(new td(new b("Executed")).setAlign("right"))
							.addElement(new td(""+project.getExecutedLinesCount()).setAlign("left"))))))
			.addElement(new tr()
				.addElement(new td("Total:").setVAlign("top")
					.addElement(Entities.NBSP)
					.addElement(new b(formatRate(project.getCoverage())))
					.addElement(Entities.NBSP)
					.addElement(createBar(project.getCoverage())))
				.addElement(new td()));
		doc.appendBody(new div().addElement(headerView).setClass("headerView"));
		doc.appendBody(new br());

		// source code
		table fileView = new table();
		fileView.setCellPadding(2).setCellSpacing(0).setClass("fileView");
		tbody fileViewBody = new tbody();
		fileView.addElement(fileViewBody);
		
		fileViewBody
			.addElement(new tr()
				.addElement(new th("Filename"))
				.addElement(new th("Branches Executed"))
				.addElement(new th("Branches Taken"))
				.addElement(new th("Calls"))
				.addElement(new th("Total").setColSpan(2)));

		Arrays.sort(files, new FileInfoCoverageComparator());
		for (int i=0; i<files.length; i++) {
			FileInfo file = files[i];
			fileViewBody
				.addElement(new tr()
					.addElement(new td(new a(file.getName()+".html", file.getName())))
					.addElement(new td(formatRate(file.getBranchesExecutedRate())).setAlign("center"))
					.addElement(new td(formatRate(file.getBranchesTakenRate())).setAlign("center"))
					.addElement(new td(formatRate(file.getCallsExecutedRate())).setAlign("center"))
					.addElement(new td(new b(formatRate(file.getCoverage()))).setAlign("right"))
					.addElement(new td(createBar(file.getCoverage()))));
		}
		doc.appendBody(fileView);

		writeDocument(doc, "overview.html");
	}

	public static Element createBar(double value) {
		if (value <= 0.0) {
			return new img().setSrc("red.png").setHeight(12).setWidth(barLength).setAlt("red");
		}
		else if (value >= 1.0) {
			return new img().setSrc("green.png").setHeight(12).setWidth(barLength).setAlt("green");
		}
		else {
			int greenWidth = (int)(value * barLength);
			return new img().setSrc("green.png").setHeight(12).setWidth(greenWidth).setAlt("green")
				.addElement(new img().setSrc("red.png").setHeight(12).setWidth(barLength - greenWidth).setAlt("red"));
		}
	}

	public static String formatRate(double rate) {
		return (rate < 0.0) ? "-" : Formatter.formatNumber(rate * 100, 2) + "%";
	}

	public static class FileInfoCoverageComparator implements Comparator {
		
		public int compare(Object o1, Object o2) 
		{
			double diff = ((FileInfo)o1).getCoverage() - ((FileInfo)o2).getCoverage();
			return (int)(diff * 1000.0);
		}

	}


}
