/*
 * gcover
 * TODO
 * Created on 14.07.2003
 *
 */
package gcover;

import gcover.output.*;
import gcover.util.Formatter;

/**
 * GCover
 * 
 * @author Tammo van Lessen
 * @version $Id: GCover.java,v 1.1 2003/07/15 18:06:32 vanto Exp $
 */
public class GCover {

	public static void main(String[] args) {
		Project prj = new Project();
		Builder.parseDir(prj, args[0]);
		
		System.out.println("Files:");
		for (int i=0; i<prj.getFiles().length; i++) {
			System.out.println(prj.getFiles()[i].getName()+":");
			System.out.println("\tcoverage: "+Formatter.formatNumber(prj.getFiles()[i].getCoverage()*100,2)+"%");
			System.out.println("\tinstrumented lines: "+prj.getFiles()[i].getInstrumentedLinesCount());
			System.out.println("\texecuted lines: "+prj.getFiles()[i].getExecutedLinesCount());
		}
		
		System.out.println();
		System.out.println("Directories:");
		for (int i=0; i<Package.getPackages().length; i++) {
			System.out.println(Package.getPackages()[i].getName()+":");
			System.out.println("\tcoverage: "+Formatter.formatNumber(Package.getPackages()[i].getCoverage()*100,2)+"%");
			System.out.println("\tinstrumented lines: "+Package.getPackages()[i].getInstrumentedLinesCount());
			System.out.println("\texecuted lines: "+Package.getPackages()[i].getExecutedLinesCount());
		}
		System.out.println();
		System.out.println("Summary:");
		System.out.println("\tcoverage: "+Formatter.formatNumber(prj.getCoverage()*100,2)+"%");
		System.out.println("\tinstrumented lines: "+prj.getInstrumentedLinesCount());
		System.out.println("\texecuted lines: "+prj.getExecutedLinesCount());
		Outputter out = new XHTMLOutputter(args[1]);
		out.output(prj);
	}
}
