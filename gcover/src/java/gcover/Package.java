/*
 * gcover
 * TODO
 * Created on 14.07.2003
 *
 */
package gcover;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Package
 * 
 * @author Tammo van Lessen
 * @version $Id: Package.java,v 1.1 2003/07/15 18:06:32 vanto Exp $
 */
public class Package {
	private String shortName;
	private String name;
	private List files = new ArrayList();
	private int instrumentedLines = 0;
	private int executedLines = 0;
 
	private static Map packages = new HashMap();
	
	private Package(String name) {
		this.name = name;
		this.shortName = name;
	}
	
	public static Package getPackage(String name) {
		Package pkg = (Package)packages.get(name);
		if (pkg == null) {
			pkg = new Package(name);
			packages.put(name, pkg);
		}
		return pkg;
	}
	
	public static Package[] getPackages() {
		return (Package[]) packages.values().toArray(new Package[0]);
	}
	
	public static void shortenNames() {
		//TODO find common prefix and drop it from all shortNames
	}
	
	/**
	 * @return
	 */
	public int getExecutedLinesCount() {
		return executedLines;
	}

	/**
	 * @return
	 */
	public FileInfo[] getFiles() {
		return (FileInfo[])files.toArray(new FileInfo[0]);
	}

	public void addFile(FileInfo file) {
		files.add(file);
		instrumentedLines += file.getInstrumentedLinesCount();
		executedLines += file.getExecutedLinesCount();
	}
	/**
	 * @return
	 */
	public int getInstrumentedLinesCount() {
		return instrumentedLines;
	}

	/**
	 * @return
	 */
	public String getName() {
		return name;
	}

	public String getShortenedName() {
		return shortName;
	}
	
	public double getCoverage() {
		return (double)executedLines / (double)instrumentedLines;
	}
}
