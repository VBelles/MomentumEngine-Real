/*
DetachElements v0.1

created: 2004-09-01
edited:
by: Chris Harvey
harvey@studio4031.com

	FEATURES:
	- is a script that seperates all the elements of a selected object into seperate objects themselves.
	- will work on editable polys, editable splines and editable meshes
	- renames the newly created objects based on the originals name
	- detaching function for splines is different from that of meshes and polys (does not center pivot/reset object)

	TO DO:
	- you can simply use the explode feature in an editable mesh with the tolerance angle set to 180
	- add seperate version for not deleting original
	- maybe create a new script just for spline hair seperation and then make the reseting of splines match that of polys and meshes

*/

MacroScript DetachElements category:"HacksawTOOLS" tooltip:"Split elements into seperate objects" ButtonText:"DetachElements"
(
on isEnabled return
(selection.count >=1)

on execute do
(--start execute loop

--***********************************************************************************************************   FUNCTION DEFINITIONS START	
	local DetachPolyElements_fn, DetachSplineElements_fn, DetachMeshElements_fn
	
	FN DetachPolyElements_fn SourceOBJ =
	(--start function for detaching elements from an editable poly object
		MasterObj = SourceOBJ 
		faceCount = MasterObj.getnumfaces()
		
		FaceTest = ((MasterObj.getnumfaces()) != 0)
		
		for ii in 1 to faceCount while FaceTest do	--ends loop when there are no faces left to select
		(--start element detach loop
			newName = (uniquename (MasterObj.name as string))
			
			MasterObj.EditablePoly.SetSelection #Face #{1}
			MasterObj.selectElement()
			TargetElement = polyop.getFaceSelection MasterObj
			polyop.detachFaces MasterObj TargetElement asNode:true name:newName
			
			NewObj = getnodebyname newName
			resetXForm NewObj 
			convertTo NewObj (Editable_Poly)
			ResetTransform NewObj 
			ResetScale NewObj 
			ResetPivot NewObj 
			centerpivot NewObj
			FaceTest = ((MasterObj.getnumfaces()) != 0)
		)--end element detach loop
		delete MasterObj	
	)--end function for detaching elements from an editable poly object
	
	FN DetachSplineElements_fn SourceOBJ =
	(--start function for detaching from editable splines
		MasterSpline = SourceOBJ 
		ns = numsplines MasterSpline 	
	
		for i in 1 to ns do    -- for each element in the spline it re-orders the elements
		(
			tempMaster = copy MasterSpline 
			tempMaster.name = (MasterSpline.name + "_Element_" + (i as string))
			select tempMaster 
			setFirstSpline tempMaster i
			
			for ii = ns to 2 by -1 do    -- deletes all of the elements except the first one
			(
				deleteSpline tempMaster ii
			)
			select MasterSpline
		)
		delete MasterSpline	
	)--end function for detaching from editable splines
	
	FN DetachMeshElements_fn SourceOBJ =
	(--start function for detaching elements from an editable Mesh object
		MasterObj = SourceOBJ
		convertTo MasterObj (Editable_Poly) 
		faceCount = MasterObj.getnumfaces()
		
		FaceTest = ((MasterObj.getnumfaces()) != 0)
		
		for ii in 1 to faceCount while FaceTest do	--ends loop when there are no faces left to select
		(--start element detach loop
			newName = (uniquename (MasterObj.name as string))
			
			MasterObj.EditablePoly.SetSelection #Face #{1}
			MasterObj.selectElement()
			TargetElement = polyop.getFaceSelection MasterObj
			polyop.detachFaces MasterObj TargetElement asNode:true name:newName
			
			NewObj = getnodebyname newName
			resetXForm NewObj 
			convertTo NewObj (Editable_Mesh)
			ResetTransform NewObj 
			ResetScale NewObj 
			ResetPivot NewObj 
			centerpivot NewObj
			FaceTest = ((MasterObj.getnumfaces()) != 0)
		)--end element detach loop
		delete MasterObj	
	)--end function for detaching elements from an editable Mesh object
--***********************************************************************************************************   FUNCTION DEFINITIONS END	
	
	max create mode
	ObjsSource = getCurrentSelection()	--create array of all selected objects
	if ObjsSource[1] == undefined then messagebox "Nothing is selected!"    -- checks to see something is selected
			
	else for obj in 1 to ObjsSource.count do with redraw off
	(--start Object loop
		SourceOBJ = ObjsSource[obj]
		if (classof SourceOBJ  == editable_poly) then (DetachPolyElements_fn SourceOBJ)			-- operates only on those objects that are editable polys
		else if (classOf SourceOBJ == SplineShape or classOf SourceOBJ == line) then (DetachSplineElements_fn SourceOBJ)    -- operates only on those objects that are splines
		else if (classOf SourceOBJ == Editable_Mesh) then (DetachMeshElements_fn SourceOBJ)		-- operates only on those objects that are editable meshes
	)--end Object loop
	max views redraw
)--end execute loop
)