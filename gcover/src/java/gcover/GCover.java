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
 * @version $Id: GCover.java,v 1.2 2004/01/07 16:00:10 squig Exp $
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
		
		// generate output
		Outputter out = new XHTMLOutputter(outputDir);
		out.output(project);

		// copy resources
		
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
