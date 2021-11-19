import React from 'react'
import {
  View,
  StyleSheet,
  Text,
  Modal,
  TouchableWithoutFeedback
} from 'react-native'

const OptionModal = ({ visible, currentItem, onClose, onPlayPress }) => {
  const { filename } = currentItem

  return (
    <>
      <Modal animationType='slide' transparent visible={visible}>
        <View style={styles.modal}>
          <Text style={styles.title} numberOfLines={2}> {filename} </Text>
          <View style={styles.optionContainer}>
            <TouchableWithoutFeedback onPress={onPlayPress}>
              <Text style={styles.option}> Play </Text>
            </TouchableWithoutFeedback>
          </View>
        </View>

        <TouchableWithoutFeedback onPress={onClose}>
          <View style={styles.modalBg} />
        </TouchableWithoutFeedback>

      </Modal>
    </>
  )
}

export default OptionModal

const styles = StyleSheet.create({
  modal: {
    position: 'absolute',
    bottom: 0,
    right: 0,
    left: 0,
    backgroundColor: '#fff',
    borderTopLeftRadius: 20,
    borderTopRightRadius: 20,
    zIndex: 1000,
  },

  optionContainer: {
    padding: 20,
  },

  title: {
    fontSize: 18,
    fontWeight: 'bold',
    padding: 20,
    paddingBottom: 0,
    color: '#303d49',
  },

  option: {
    fontSize: 16,
    fontWeight: 'bold',
    color: '#636363',
    paddingVertical: 10,
  },

  modalBg: {
    position: 'absolute',
    top: 0,
    right: 0,
    left: 0,
    bottom: 0,
  },
})