/*
 * gcover
 * TODO
 * Created on 14.07.2003
 *
 */
package gcover;

import java.util.ArrayList;
import java.util.*;
import java.util.List;
import java.util.Map;

/**
 * Project
 * 
 * @author Tammo van Lessen
 * @version $Id: Project.java,v 1.2 2004/01/11 12:09:30 squig Exp $
 */
public class Project {

	private List files = new ArrayList();
	private Map packages = new HashMap();
	private int instrumentedLines = 0;
	private int executedLines = 0;

	public FileInfo[] getFiles() {
		FileInfo[] array = (FileInfo[])files.toArray(new FileInfo[0]);
		Arrays.sort(array);
		return array;
	}
	
	public void addFile(FileInfo file) {
		files.add(file);
		instrumentedLines += file.getInstrumentedLinesCount();
		executedLines += file.getExecutedLinesCount();
	}
	
	public int getInstrumentedLinesCount() {
		return instrumentedLines;
	}
	
	public int getExecutedLinesCount() {
		return executedLines;
	}
	
	public double getCoverage() {
		return (double)executedLines / (double)instrumentedLines;
	}
}
