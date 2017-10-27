/*
 * JPIB
 *
 *  Copyright (C) 2016 Ralf Tralow (www.ringwelt.de)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * For current versions and more information, please visit:
 * 
 *
 *  More projects on:
 *  http://www.ringwelt.de
 */

package be.ac.ulb.gpib;

import java.io.File;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Manager for start- and application directories.
 *
 * @author nigjo
 */
public class DirectoryManager {
  public static File startdir;
  public static File appdir;

  public static void init(Class<?> mainClass) {
    String userdir = System.getProperty("user.dir");
    startdir = new File(userdir);

    String mainResName = mainClass.getSimpleName()+".class";
    URL mainRes = mainClass.getResource(mainResName);
    if(!mainRes.getProtocol().equals("jar")) {
      // don't know what to do. use startdir.
      appdir = startdir;
      return;
    }
    String mainLocator = mainRes.getPath();
    String jarpart = mainLocator.substring(0,
        mainLocator.lastIndexOf("!/"));
    File jarfile;
    try
    {
      jarfile = new File(new URI(jarpart));
      appdir = jarfile.getParentFile();
    }
    catch(URISyntaxException ex)
    {
      // should not happen. actually.
      Logger.getLogger(DirectoryManager.class.getName()).log(
          Level.SEVERE, null, ex);
      appdir = startdir;
    }
  }
}