/*
 *  GCover
 *
 *  Copyright by Steffen Pingel
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
package gcover.util;

import java.io.*;

/**
 * Provides static helper methods.
 *
 * @author Steffen Pingel
 */
public class Util {

	/**
	 * Copies <code>source</code> to <code>dest</code>. If dest already exists
	 * it is overwritten.
	 *
	 * @param source the source file
	 * @param dest the destination file
	 */
	public static void copy(InputStream inStream, OutputStream outStream)
		throws IOException 
	{
		BufferedInputStream in = null;
		BufferedOutputStream out = null;
		try {
			in = new BufferedInputStream(inStream);
			out = new BufferedOutputStream(outStream);

			byte buffer[] = new byte[512 * 1024];
			int count;
			while ((count = in.read(buffer, 0, buffer.length)) != -1) {
				out.write(buffer, 0, count);
			}
			out.flush();
		}
		finally {
			if (in != null) {
				try {
					in.close();
				}
				catch (IOException e) {
				}
			}
			if (out != null) {
				try {
					out.close();
				}
				catch (IOException e) {
				}
			}	   
		}
	}

}
