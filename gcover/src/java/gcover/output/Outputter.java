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
 */
package gcover.output;

import gcover.Project;

/**
 * Outputter
 * 
 * @author Tammo van Lessen
 * @version $Id: Outputter.java,v 1.2 2004/01/07 17:01:38 squig Exp $
 */
public interface Outputter {

	void output(Project prj);

	void copyResources();

}
