<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.11.2" name="guy" tilewidth="35" tileheight="39" tilecount="15" columns="5">
 <image source="guy.png" width="175" height="117"/>
 <tile id="0">
  <objectgroup draworder="index" id="2">
   <object id="1" x="6.75" y="2.5" width="21.625" height="35.75"/>
   <object id="2" type="zcollider" x="9.5" y="35.375" width="9.75" height="2.75"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="200"/>
   <frame tileid="1" duration="200"/>
   <frame tileid="2" duration="200"/>
   <frame tileid="3" duration="200"/>
   <frame tileid="5" duration="200"/>
  </animation>
 </tile>
 <tile id="6">
  <objectgroup draworder="index" id="2">
   <object id="1" x="4.36364" y="3.45455" width="24" height="35.0909"/>
   <object id="2" type="ycollider" x="4.25" y="33" width="22.25" height="5.25"/>
  </objectgroup>
  <animation>
   <frame tileid="6" duration="10"/>
   <frame tileid="7" duration="10"/>
   <frame tileid="8" duration="10"/>
   <frame tileid="9" duration="10"/>
  </animation>
 </tile>
 <tile id="10">
  <objectgroup draworder="index" id="2">
   <object id="1" x="6.36364" y="0" width="21.6364" height="39"/>
  </objectgroup>
  <animation>
   <frame tileid="10" duration="200"/>
   <frame tileid="11" duration="200"/>
  </animation>
 </tile>
</tileset>
