package be.belgium.eid;

import java.util.*;

public class LabelsBundle_de extends ListResourceBundle
{
  static final Object[][] contents =
  {
      {"AppLabel", "Eine Netzanwendung m�chte die Karte zug�nglich machen !"},
      {"Function", "Funktion:"},
      {"URL", "URL:"},
      {"Accept", "M�chten Sie es annehmen ?"},
      {"Yes", "Ja"},
      {"No", "Nein"},
      {"Title", "eID Karte Zugang Best�tigung"},
      {"ReadPic", "Lesen Sie Abbildung Daten"},
      {"ReadID", "Lesen Sie Identit�t Daten"},
      {"ReadAddr", "Lesen Sie Adresse Daten"},
      {"ReadRaw", "Lesen Sie rohe Daten"}
  };

  public Object[][] getContents()
  {
    return contents;
  }
}
