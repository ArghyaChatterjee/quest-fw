import wx
import GDSChannelFilterDialogGUI
import csv

###########################################################################
## Class ChannelFilterDialogImpl
###########################################################################

class ChannelFilterDialogImpl(GDSChannelFilterDialogGUI.ChannelFilterDialog):
    
    def __init__( self, parent, ch_dict ):

        GDSChannelFilterDialogGUI.ChannelFilterDialog.__init__ ( self, parent)
        self.ch_dict = ch_dict
        self.filter = []
        self.current_filter_name = ""
        self.ListBoxAllChannels.SetItems([c.get_full_name() for c in ch_dict.values()])

        # List of channel templates that should be displayed with the current filter


    def Swap(self, dest, src, all_itms=False):

        if all_itms:
            itms = src.Items
            dest.AppendItems(itms)
            src.Clear()
        else:
            idxs = src.GetSelections()
            itms = [src.GetString(i) for i in idxs]
            dest.AppendItems(itms)
            for i in itms:
                src.Delete(src.Items.index(i))
        
    def GetFilter(self):
        return self.filter
            
    # Virtual event handlers, overide them in your derived class
    def onCloseChannelFilterDialog( self, event ):
        self.EndModal(-1)
	
    def onClickAddChannel( self, event ):
        self.Swap(self.ListBoxShowChannels, self.ListBoxAllChannels)
        event.Skip()
    
    def onClickAddAllChannels( self, event ):
        self.Swap(self.ListBoxShowChannels, self.ListBoxAllChannels, all_itms=True)
        event.Skip()
    
    def onClickRemoveChannel( self, event ):
        self.Swap(self.ListBoxAllChannels, self.ListBoxShowChannels)
        event.Skip()
    
    def onClickRemoveAllChannels( self, event ):
        self.Swap(self.ListBoxAllChannels, self.ListBoxShowChannels, all_itms=True)
        event.Skip()
    
    def onClickSaveFilter( self, event ):
        dialog = wx.FileDialog(self, "Save filter", wildcard="CSV files (*.csv)|*.csv", style=wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT) 
        ret = dialog.ShowModal()
        print(ret)
        if ret == wx.ID_OK:
            f = dialog.GetPath()
            with open(f, 'wb') as csvfile:
                w = csv.writer(csvfile)
                w.writerow(self.ListBoxShowChannels.Items)
            self.TextCtrlFileName.SetValue(dialog.GetFilenames()[0])
            self.current_filter_name = dialog.GetFilenames()[0]
        event.Skip()
    
    def onClickLoadFilter( self, event ):
        dialog = wx.FileDialog(self, "Open filter", wildcard="CSV files (*.csv)|*.csv", style=wx.FD_OPEN | wx.FD_FILE_MUST_EXIST) 
        ret = dialog.ShowModal()
        if ret == wx.ID_OK:
            f = dialog.GetPath()
            with open(f, 'rb') as csvfile:
                r = csv.reader(csvfile)
                self.filter = r.next()
            self.TextCtrlFileName.SetValue(dialog.GetFilenames()[0])
            self.current_filter_name = dialog.GetFilenames()[0]
            self.ListBoxShowChannels.SetItems(self.filter)
            self.ListBoxAllChannels.SetItems([c.get_full_name() for c in self.ch_dict.values() if c.get_full_name() not in self.filter])
        event.Skip()
    
    def onClickApplyFilter( self, event ):
        self.filter = self.ListBoxShowChannels.Items
        self.EndModal(0)

    def onClickClose( self, event ):
        self.EndModal(-1)