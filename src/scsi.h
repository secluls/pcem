struct scsi_bus_t;

typedef struct scsi_device_t
{
        void *(*init)(struct scsi_bus_t *bus, int id);
        void (*close)(void *p);
        
        void (*start_command)(void *p);
        int (*command)(uint8_t *cdb, void *p);
        
        uint8_t (*get_status)(void *p);
        
        uint8_t (*read)(void *p);
        void (*write)(uint8_t val, void *p);
        int (*read_complete)(void *p);
        int (*write_complete)(void *p);
} scsi_device_t;

typedef struct scsi_bus_t
{
        int state;
        int new_state;
        int clear_req;
        uint32_t bus_in, bus_out;
        int dev_id;

        int command_pos;
        uint8_t command[20];
        
        scsi_device_t *devices[8];
        void *device_data[8];
        
        int change_state_delay;
        int new_req_delay;
} scsi_bus_t;

//int scsi_add_data(uint8_t val);
//int scsi_get_data();
void scsi_set_phase(uint8_t phase);
void scsi_set_irq(uint8_t status);

#define SCSI_TEST_UNIT_READY              0x00
#define SCSI_REZERO_UNIT                  0x01
#define SCSI_REQUEST_SENSE                0x03
#define SCSI_FORMAT                       0x04
#define SCSI_READ_6                       0x08
#define SCSI_WRITE_6                      0x0a
#define SCSI_SEEK_6                       0x0b
#define SCSI_INQUIRY                      0x12
#define SCSI_MODE_SELECT_6                0x15
#define SCSI_MODE_SENSE_6                 0x1a
#define SCSI_START_STOP_UNIT              0x1b
#define SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL 0x1e
#define SCSI_READ_CAPACITY_10             0x25
#define SCSI_READ_10                      0x28
#define SCSI_WRITE_10                     0x2a
#define SCSI_SEEK_10                      0x2b
#define SCSI_VERIFY_10                    0x2f
#define SCSI_READ_BUFFER                  0x3c

#define SCSI_RESERVE                      0x16
#define SCSI_RELEASE                      0x17
#define SEND_DIAGNOSTIC                   0x1d

#define STATUS_GOOD            0x00
#define STATUS_CHECK_CONDITION 0x02

#define MSG_COMMAND_COMPLETE 0x00

#define BUS_DBP 0x01
#define BUS_SEL 0x02
#define BUS_IO  0x04
#define BUS_CD  0x08
#define BUS_MSG 0x10
#define BUS_REQ 0x20
#define BUS_BSY 0x40
#define BUS_RST 0x80
#define BUS_ACK 0x200
#define BUS_ATN 0x200
#define BUS_ARB 0x8000
#define BUS_SETDATA(val) ((uint32_t)val << 16)
#define BUS_GETDATA(val) ((val >> 16) & 0xff)
#define BUS_DATAMASK 0xff0000

#define BUS_IDLE (1 << 31)

int scsi_bus_update(scsi_bus_t *bus, int bus_assert);
int scsi_bus_read(scsi_bus_t *bus);
int scsi_bus_match(scsi_bus_t *bus, int bus_assert);
void scsi_bus_kick(scsi_bus_t *bus);
void scsi_bus_init(scsi_bus_t *bus);
void scsi_bus_close(scsi_bus_t *bus);

#define KEY_ILLEGAL_REQ 5

#define ASC_INVALID_LUN 0x25

#define SCSI_PHASE_DATA_OUT    0
#define SCSI_PHASE_DATA_IN     BUS_IO
#define SCSI_PHASE_COMMAND     BUS_CD
#define SCSI_PHASE_STATUS      (BUS_CD | BUS_IO)
#define SCSI_PHASE_MESSAGE_OUT (BUS_MSG | BUS_CD)
#define SCSI_PHASE_MESSAGE_IN  (BUS_MSG | BUS_CD | BUS_IO)