/*
 * gcover
 * TODO
 * Created on 14.07.2003
 *
 */
package gcover;

import gcover.util.Formatter;

import java.util.ArrayList;
import java.util.List;

/**
 * File
 * 
 * @author Tammo van Lessen
 * @version $Id: FileInfo.java,v 1.4 2004/01/14 15:33:15 squig Exp $
 */
public class FileInfo implements Comparable {

	private int executions = 0;
	private String packageName;
	private int instrumentedLines = 0;
	private int executedLines = 0;
	private String filename;
	private List lines = new ArrayList();
	
	private double branchesExecutedRate = -1;
	private double branchesTakenRate = -1;
	private int branchesTotal = -1;
	private double callsExecutedRate = -1;
	private int callsTotal = -1;
	
	public FileInfo(String packageName, String filename) {
		this.filename = filename;
		this.packageName = packageName;
	}
	
	/**
	 * @return
	 */
	public LineInfo[] getLines() {
		return (LineInfo[])lines.toArray(new LineInfo[0]);
	}

	/**
	 * @return the filename without path
	 */
	public String getName() {
		return filename;
	}

	/**
	 * @param list
	 */
	public void addLine(LineInfo line) {
		lines.add(line);
		if (line.isInstrumented()) {
			instrumentedLines++;
		}
		if (line.getExecCount() != 0) {
			executedLines++;
		}
		executions += line.getExecCount();
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

	public int compareTo(Object o) 
	{
		return filename.compareTo(((FileInfo)o).filename);
	}

	public String toString() {
		return packageName+":"+filename+" ("+ Formatter.formatNumber(getCoverage()*200,2) +"% covered)";
	}

	public String getPackageName() {
		return packageName;
	}

	public int getExecutionCount() {
		return executions;
	}

	public int getLinesOfCode()
	{
		return lines.size();
	}

	public void setBranchesExecutedRate(double branchesExecutedRate) {
		this.branchesExecutedRate = branchesExecutedRate;
	}

	public double getBranchesExecutedRate() {
		return branchesExecutedRate;
	}

	public void setBranchesTakenRate(double branchesTakenRate) {
		this.branchesTakenRate = branchesTakenRate;
	}

	public double getBranchesTakenRate() {
		return branchesTakenRate;
	}

	public void setBranchesTotal(int branchesTotal) {
		this.branchesTotal = branchesTotal;
	}

	public int getBranchesTotal() {
		return branchesTotal;
	}

	public void setCallsExecutedRate(double callsExecutedRate) {
		this.callsExecutedRate = callsExecutedRate;
	}

	public double getCallsExecutedRate() {
		return callsExecutedRate;
	}

	public void setCallsTotal(int callsTotal) {
		this.callsTotal = callsTotal;
	}

	public int getCallsTotal() {
		return callsTotal;
	}

}
