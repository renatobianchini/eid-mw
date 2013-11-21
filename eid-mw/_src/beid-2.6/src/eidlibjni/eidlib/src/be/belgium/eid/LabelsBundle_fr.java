package be.belgium.eid;

import java.util.*;

public class LabelsBundle_fr extends ListResourceBundle
{
  static final Object[][] contents =
    {
        {"AppLabel", "Une application Web veut acc�der � la carte !"},
        {"Function", "Fonction:"},
        {"URL", "URL:"},
        {"Accept", "Voulez-vous l'accepter ?"},
        {"Yes", "Oui"},
        {"No", "Non"},
        {"Title", "Confirmation d'acc�s � la carte eID"},
        {"ReadPic", "Lire la photo"},
        {"ReadID", "Lire les donn�es d'identit�"},
        {"ReadAddr", "Lire l'adresse"},
        {"ReadRaw", "Lire des donn�es non sp�cifi�es"}
    };

    public Object[][] getContents()
    {
      return contents;
    }
}
