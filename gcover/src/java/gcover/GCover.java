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
package gcover;

import gcover.output.*;
import gcover.util.Formatter;

/**
 * GCover
 * 
 * @author Tammo van Lessen
 * @author Steffen Pingel
 * @version $Id: GCover.java,v 1.4 2004/01/11 16:01:34 squig Exp $
 */
public class GCover {

	public static void main(String[] args) {
		if (args.length >= 1 
			&& (args[0].equals("-h") || args[0].equals("--help"))) {

			printHelp();
			System.exit(0);
		}
		else if (args.length < 2) { 
			printHelp();
			System.exit(1);
		}

		run(args[0], args[1]);
	}

	public static void run(String inputDir, String outputDir)
	{
		Project project = new Project();

		// build
		Builder.parseDir(project, inputDir);

		// debug
		//printDebug(project);
		
		// generate output
		Outputter out = new XHTMLOutputter(outputDir);
		out.output(project);		
		out.copyResources();
	}

	public static void printDebug(Project prj)
	{
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
	}

	public static void printHelp()
	{
		System.err.println("usage: java -jar gcover.jar input-path output-path");
		System.err.println();
		System.err.println("       input-path  - the directory containing .gcov files");
		System.err.println("       output-path - the directory that will contain the html files ");
		System.err.println();
	}

}
